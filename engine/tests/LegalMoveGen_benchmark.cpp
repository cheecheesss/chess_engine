// собрал чат гпт
// Standalone benchmark for legal move generation.
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>

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
	std::uint64_t iterations;
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

std::uint64_t hashMoves(const MoveList& moves) {
	std::uint64_t hash = 14695981039346656037ULL;
	for (const Move& move : moves) {
		hash ^= static_cast<std::uint64_t>(move.from + 1);
		hash *= 1099511628211ULL;
		hash ^= static_cast<std::uint64_t>(move.to + 1);
		hash *= 1099511628211ULL;
	}
	return hash ^ static_cast<std::uint64_t>(moves.size());
}

Sample runSample(
	const MoveGenerator& generator,
	const BenchmarkPosition& benchmarkPosition,
	std::chrono::duration<double> duration
) {
	constexpr std::uint64_t batchSize = 128;
	std::uint64_t calls = 0;
	std::uint64_t totalMoves = 0;
	std::uint64_t checksum = 0;
	const auto start = Clock::now();
	const auto deadline = start + duration;

	do {
		for (std::uint64_t batchCall = 0; batchCall < batchSize; ++batchCall) {
			const MoveList moves = generator.generateLegalMovesForSide(
				benchmarkPosition.position,
				benchmarkPosition.sideToMove
			);
			totalMoves += moves.size();
			checksum ^= hashMoves(moves) + calls * 0x9e3779b97f4a7c15ULL;
			++calls;
		}
	} while (Clock::now() < deadline);

	const double elapsed = std::chrono::duration<double>(Clock::now() - start).count();
	return {
		static_cast<double>(calls) / elapsed,
		static_cast<double>(totalMoves) / elapsed,
		static_cast<double>(totalMoves) / static_cast<double>(calls),
		calls,
		checksum
	};
}

double medianRate(
	const std::array<Sample, 5>& samples,
	double Sample::*rate
) {
	std::array<double, 5> rates{};
	for (std::size_t index = 0; index < samples.size(); ++index) {
		rates[index] = samples[index].*rate;
	}
	std::sort(rates.begin(), rates.end());
	return rates[rates.size() / 2];
}

int main(int argc, char** argv) {
	double secondsPerSample = 0.5;
	if (argc > 1) {
		char* parseEnd = nullptr;
		secondsPerSample = std::strtod(argv[1], &parseEnd);
		if (parseEnd == argv[1] || *parseEnd != '\0' ||
			!std::isfinite(secondsPerSample) || secondsPerSample <= 0.0) {
			std::cerr << "Usage: " << argv[0] << " [seconds-per-sample > 0]\n";
			return 1;
		}
	}

	constexpr std::size_t sampleCount = 5;
	const auto sampleDuration = std::chrono::duration<double>(secondsPerSample);
	const auto warmupDuration = std::chrono::milliseconds(250);
	const std::array<BenchmarkPosition, 3> positions = {
		makeStartingPosition(),
		makeMiddlegamePosition(),
		makeEndgamePosition()
	};
	const MoveGenerator generator;

	const MoveList startMoves = generator.generateLegalMovesForSide(
		positions[0].position,
		positions[0].sideToMove
	);
	if (startMoves.size() != 20) {
		std::cerr << "Start-position sanity check failed: expected 20 legal moves, got "
				  << startMoves.size() << '\n';
		return 2;
	}

	std::cout << "Legal move generation benchmark: " << sampleCount
			  << " samples x " << secondsPerSample << " s, warmup "
			  << warmupDuration.count() << " ms\n";
	for (const BenchmarkPosition& benchmarkPosition : positions) {
		runSample(generator, benchmarkPosition, warmupDuration);

		std::array<Sample, sampleCount> samples{};
		for (Sample& sample : samples) {
			sample = runSample(generator, benchmarkPosition, sampleDuration);
		}

		double averageMoves = 0.0;
		std::uint64_t checksum = 0;
		for (const Sample& sample : samples) {
			averageMoves += sample.averageMoves;
			checksum ^= sample.checksum;
		}
		averageMoves /= static_cast<double>(sampleCount);

		std::cout << std::left << std::setw(18) << benchmarkPosition.name
				  << " median: " << std::right << std::fixed << std::setprecision(0)
				  << medianRate(samples, &Sample::generationsPerSecond)
				  << " generations/s, "
				  << medianRate(samples, &Sample::movesPerSecond)
				  << " moves/s, avg legal moves: "
				  << std::setprecision(2) << averageMoves
				  << ", checksum: " << std::hex << checksum << std::dec << '\n';
	}
}
