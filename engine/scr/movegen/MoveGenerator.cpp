#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

/// @brief Генерирует псевдо-законные ходы для фигуры на указанной позиции
/// @param position 
/// @param from 
/// @return 
std::vector<Move> MoveGenerator::generatePseudoLegalMoves(
    const Position& position, // list 0-63 (squares of board)
    int from // move from coord
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    Piece piece = position.at(from);
    if (piece.isEmpty()) {
        return {};
    }

    switch (piece.type) {
        case PieceType::Pawn:
            return generatePawnMoves(position, from);

        case PieceType::Knight:
            return generateJumpMoves(position, from);

        case PieceType::King: {
            std::vector<Move> moves = generateJumpMoves(position, from);
            for (CastlingSide side : {CastlingSide::KingSide, CastlingSide::QueenSide}) {
                std::vector<Move> castlingMoves =
                    generateCastlingMoves(position, from, side);
                moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());
            }
            return moves;
        }

        case PieceType::Bishop:
        case PieceType::Rook:
        case PieceType::Queen:
            return generateSlidingMoves(position, from);
        
        default: return {};
    }   
}

