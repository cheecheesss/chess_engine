// Standalone benchmark and sanity checks for legal move generation.
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "movegen/MoveGenerator.hpp"

using Clock = std::chrono::steady_clock;

struct BenchmarkPosition {
	const char* name;
	Position position;
	Color sideToMove;
};

struct Sample {
	double generationsPerSecond;
	double movesPerSecond;
	double averageMoves;
	std::uint64_t checksum;
};

void put(Position& position, int square, PieceType type, Color color) {
	position.setPiece(square, Piece{type, color});
}

BenchmarkPosition makeStartingPosition() {
	Position position;
	constexpr std::array<PieceType, 8> backRank = {
		PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
		PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook
	};

	for (int file = 0; file < 8; ++file) {
		put(position, file, backRank[file], Color::Black);
		put(position, 8 + file, PieceType::Pawn, Color::Black);
		put(position, 48 + file, PieceType::Pawn, Color::White);
		put(position, 56 + file, backRank[file], Color::White);
	}

	position.setCastlingRight(Color::White, CastlingSide::KingSide, true);
	position.setCastlingRight(Color::White, CastlingSide::QueenSide, true);
	position.setCastlingRight(Color::Black, CastlingSide::KingSide, true);
	position.setCastlingRight(Color::Black, CastlingSide::QueenSide, true);
	return {"start position", position, Color::White};
}

BenchmarkPosition makeMiddlegamePosition() {
	Position position;
	for (int square : {0, 7}) put(position, square, PieceType::Rook, Color::Black);
	for (int square : {2, 14}) put(position, square, PieceType::Bishop, Color::Black);
	for (int square : {21, 18}) put(position, square, PieceType::Knight, Color::Black);
	put(position, 3, PieceType::Queen, Color::Black);
	put(position, 6, PieceType::King, Color::Black);
	for (int square : {8, 9, 10, 19, 28, 13, 22, 15}) {
		put(position, square, PieceType::Pawn, Color::Black);
	}

	put(position, 56, PieceType::Rook, Color::White);
	put(position, 61, PieceType::Rook, Color::White);
	put(position, 34, PieceType::Bishop, Color::White);
	put(position, 54, PieceType::Bishop, Color::White);
	put(position, 42, PieceType::Knight, Color::White);
	put(position, 45, PieceType::Knight, Color::White);
	put(position, 51, PieceType::Queen, Color::White);
	put(position, 60, PieceType::King, Color::White);
	for (int square : {48, 49, 50, 43, 36, 53, 46, 55}) {
		put(position, square, PieceType::Pawn, Color::White);
	}

	return {"middlegame", position, Color::White};
}

BenchmarkPosition makeEndgamePosition() {
	Position position;
	put(position, 56, PieceType::Rook, Color::White);
	put(position, 28, PieceType::King, Color::White);
	put(position, 27, PieceType::Pawn, Color::White);
	put(position, 29, PieceType::Pawn, Color::White);
	put(position, 7, PieceType::Rook, Color::Black);
	put(position, 4, PieceType::King, Color::Black);
	put(position, 11, PieceType::Pawn, Color::Black);
	put(position, 13, PieceType::Pawn, Color::Black);
	return {"rook endgame", position, Color::Black};
}

BenchmarkPosition makePromotionPosition() {
	Position position;
	put(position, 60, PieceType::King, Color::White);
	put(position, 7, PieceType::King, Color::Black);
	put(position, 8, PieceType::Pawn, Color::White);
	put(position, 1, PieceType::Rook, Color::Black);
	return {"promotion", position, Color::White};
}

bool samePosition(const Position& left, const Position& right) {
	for (int square = 0; square < 64; ++square) {
		const Piece leftPiece = left.at(square);
		const Piece rightPiece = right.at(square);
		if (leftPiece.type != rightPiece.type ||
			leftPiece.color != rightPiece.color) {
			return false;
		}
	}
	const CastlingRights leftRights = left.getCastlingRights();
	const CastlingRights rightRights = right.getCastlingRights();
	return left.enPassantTarget() == right.enPassantTarget() &&
		leftRights.whiteKingSide == rightRights.whiteKingSide &&
		leftRights.whiteQueenSide == rightRights.whiteQueenSide &&
		leftRights.blackKingSide == rightRights.blackKingSide &&
		leftRights.blackQueenSide == rightRights.blackQueenSide;
}

std::uint64_t hashMoves(const MoveList& moves) {
	std::uint64_t hash = 14695981039346656037ULL;
	for (const Move& move : moves) {
		hash ^= static_cast<std::uint64_t>(move.from + 1);
		hash *= 1099511628211ULL;
		hash ^= static_cast<std::uint64_t>(move.to + 1);
		hash *= 1099511628211ULL;
		hash ^= static_cast<std::uint64_t>(move.promotion);
		hash *= 1099511628211ULL;
	}
	return hash ^ static_cast<std::uint64_t>(moves.size());
}

bool isPromotionPiece(PieceType type) {
	return type == PieceType::Queen || type == PieceType::Rook ||
		type == PieceType::Bishop || type == PieceType::Knight;
}

void validateMoveList(
	const BenchmarkPosition& benchmarkPosition,
	const MoveList& moves
) {
	for (std::size_t index = 0; index < moves.size(); ++index) {
		const Move& move = moves.begin()[index];
		if (move.from >= 64 || move.to >= 64) {
			throw std::runtime_error("Generated move has an out-of-range square");
		}

		const Piece movingPiece = benchmarkPosition.position.at(move.from);
		const Piece targetPiece = benchmarkPosition.position.at(move.to);
		if (movingPiece.isEmpty() ||
			movingPiece.color != benchmarkPosition.sideToMove) {
			throw std::runtime_error("Generated move has an invalid origin");
		}
		if ((!targetPiece.isEmpty() &&
			 targetPiece.color == benchmarkPosition.sideToMove) ||
			targetPiece.type == PieceType::King) {
			throw std::runtime_error("Generated move captures an invalid piece");
		}

		const int promotionRank =
			movingPiece.color == Color::White ? 0 : 7;
		const bool reachesPromotionRank =
			movingPiece.type == PieceType::Pawn &&
			move.to / 8 == promotionRank;
		if (reachesPromotionRank != (move.promotion != PieceType::None) ||
			(move.promotion != PieceType::None &&
			 !isPromotionPiece(move.promotion))) {
			throw std::runtime_error("Generated move has an invalid promotion");
		}

		for (std::size_t previous = 0; previous < index; ++previous) {
			const Move& other = moves.begin()[previous];
			if (move.from == other.from && move.to == other.to &&
				move.promotion == other.promotion) {
				throw std::runtime_error("Generated move list contains a duplicate");
			}
		}
	}
}

MoveList checkedMoves(
	const MoveGenerator& generator,
	const BenchmarkPosition& benchmarkPosition
) {
	const Position originalPosition = benchmarkPosition.position;
	MoveList moves = generator.generateLegalMovesForSide(
		benchmarkPosition.position,
		benchmarkPosition.sideToMove
	);
	if (!samePosition(originalPosition, benchmarkPosition.position)) {
		throw std::runtime_error("Move generation modified its input position");
	}
	validateMoveList(benchmarkPosition, moves);
	return moves;
}

void validatePromotionOptions(
	const MoveList& moves,
	int from,
	int to,
	const char* description
) {
	constexpr std::array<PieceType, 4> expected = {
		PieceType::Queen, PieceType::Rook,
		PieceType::Bishop, PieceType::Knight
	};
	for (PieceType type : expected) {
		std::size_t matches = 0;
		for (const Move& move : moves) {
			if (move.from == from && move.to == to && move.promotion == type) {
				++matches;
			}
		}
		if (matches != 1) {
			throw std::runtime_error(
				std::string(description) + " is missing or duplicates a promotion option"
			);
		}
	}
}

void runSanityChecks(const MoveGenerator& generator) {
	const BenchmarkPosition start = makeStartingPosition();
	if (checkedMoves(generator, start).size() != 20) {
		throw std::runtime_error("Start position must have exactly 20 legal moves");
	}

	const BenchmarkPosition promotion = makePromotionPosition();
	const MoveList promotionMoves = checkedMoves(generator, promotion);
	validatePromotionOptions(promotionMoves, 8, 0, "quiet promotion");
	validatePromotionOptions(promotionMoves, 8, 1, "capture promotion");

	Position blackPromotionPosition;
	put(blackPromotionPosition, 60, PieceType::King, Color::White);
	put(blackPromotionPosition, 7, PieceType::King, Color::Black);
	put(blackPromotionPosition, 48, PieceType::Pawn, Color::Black);
	const BenchmarkPosition blackPromotion = {
		"black promotion", blackPromotionPosition, Color::Black
	};
	const MoveList blackPromotionMoves = checkedMoves(generator, blackPromotion);
	validatePromotionOptions(blackPromotionMoves, 48, 56, "black promotion");
}

Sample runSample(
	const MoveGenerator& generator,
	const BenchmarkPosition& benchmarkPosition,
	std::uint64_t iterations
) {
	std::uint64_t totalMoves = 0;
	std::uint64_t checksum = 14695981039346656037ULL;
	const auto start = Clock::now();

	for (std::uint64_t call = 0; call < iterations; ++call) {
		const MoveList moves = generator.generateLegalMovesForSide(
			benchmarkPosition.position,
			benchmarkPosition.sideToMove
		);
		totalMoves += moves.size();

		checksum ^= static_cast<std::uint64_t>(moves.size());
		checksum *= 1099511628211ULL;
		if (!moves.empty()) {
			const Move& first = moves.begin()[0];
			const Move& last = moves.begin()[moves.size() - 1];
			checksum ^= static_cast<std::uint64_t>(first.from) |
				(static_cast<std::uint64_t>(first.to) << 8) |
				(static_cast<std::uint64_t>(first.promotion) << 16);
			checksum *= 1099511628211ULL;
			checksum ^= static_cast<std::uint64_t>(last.from) |
				(static_cast<std::uint64_t>(last.to) << 8) |
				(static_cast<std::uint64_t>(last.promotion) << 16);
			checksum *= 1099511628211ULL;
		}
	}

	const double elapsed =
		std::chrono::duration<double>(Clock::now() - start).count();
	return {
		static_cast<double>(iterations) / elapsed,
		static_cast<double>(totalMoves) / elapsed,
		static_cast<double>(totalMoves) / static_cast<double>(iterations),
		checksum
	};
}

double medianRate(const std::vector<Sample>& samples, double Sample::*rate) {
	std::vector<double> rates;
	rates.reserve(samples.size());
	for (const Sample& sample : samples) {
		rates.push_back(sample.*rate);
	}
	std::sort(rates.begin(), rates.end());
	return rates[rates.size() / 2];
}

std::uint64_t parsePositiveInteger(const char* text, const char* name) {
	char* parseEnd = nullptr;
	const unsigned long long value = std::strtoull(text, &parseEnd, 10);
	if (parseEnd == text || *parseEnd != '\0' || value == 0) {
		throw std::invalid_argument(std::string(name) + " must be a positive integer");
	}
	return static_cast<std::uint64_t>(value);
}

int main(int argc, char** argv) {
	try {
		const std::uint64_t iterations =
			argc > 1 ? parsePositiveInteger(argv[1], "iterations") : 300000;
		const std::uint64_t sampleCount =
			argc > 2 ? parsePositiveInteger(argv[2], "samples") : 7;
		if (argc > 3 || sampleCount < 3 || sampleCount > 31 ||
			sampleCount % 2 == 0) {
			std::cerr << "Usage: " << argv[0]
					  << " [iterations-per-sample > 0] [odd-samples: 3..31]\n";
			return 1;
		}

		const MoveGenerator generator;
		runSanityChecks(generator);
		const std::array<BenchmarkPosition, 4> positions = {
			makeStartingPosition(),
			makeMiddlegamePosition(),
			makeEndgamePosition(),
			makePromotionPosition()
		};

		std::cout << "Sanity checks passed; sizeof(Move)=" << sizeof(Move)
				  << " bytes; " << iterations << " iterations x "
				  << sampleCount << " samples per position\n";

		for (const BenchmarkPosition& benchmarkPosition : positions) {
			const MoveList expectedMoves = checkedMoves(generator, benchmarkPosition);
			const std::uint64_t expectedHash = hashMoves(expectedMoves);
			for (int warmup = 0; warmup < 2; ++warmup) {
				runSample(generator, benchmarkPosition, iterations / 10 + 1);
			}

			std::vector<Sample> samples;
			samples.reserve(static_cast<std::size_t>(sampleCount));
			std::uint64_t checksum = 0;
			for (std::uint64_t sampleIndex = 0;
				 sampleIndex < sampleCount;
				 ++sampleIndex) {
				const Sample sample =
					runSample(generator, benchmarkPosition, iterations);
				if (sample.averageMoves !=
					static_cast<double>(expectedMoves.size())) {
					throw std::runtime_error(
						"Move count changed during benchmark for " +
						std::string(benchmarkPosition.name)
					);
				}
				samples.push_back(sample);
				checksum ^= sample.checksum;
			}

			std::cout << std::left << std::setw(18) << benchmarkPosition.name
					  << " " << std::right << std::fixed << std::setprecision(0)
					  << medianRate(samples, &Sample::generationsPerSecond)
					  << " generations/s, "
					  << medianRate(samples, &Sample::movesPerSecond)
					  << " moves/s, avg "
					  << std::setprecision(2) << expectedMoves.size()
					  << " legal moves, move hash " << std::hex << expectedHash
					  << ", run checksum " << checksum << std::dec << '\n';
		}
	} catch (const std::exception& error) {
		std::cerr << "Benchmark failed: " << error.what() << '\n';
		return 2;
	}
	return 0;
}
