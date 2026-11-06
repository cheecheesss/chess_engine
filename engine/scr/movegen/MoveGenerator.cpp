#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

/// @brief Генерирует псевдо-законные ходы для фигуры на указанной позиции
/// @param position 
/// @param from
/// @param moves
void MoveGenerator::generatePseudoLegalMoves(
    const Position& position, // list 0-63 (squares of board)
    int from, // move from coord
    std::vector<Move>& moves
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    Piece piece = position.at(from);
    if (piece.isEmpty()) {
        return;
    }

    switch (piece.type) {
        case PieceType::Pawn:
            generatePawnMoves(position, from, moves);
            break;

        case PieceType::Knight:
            generateJumpMoves(position, from, moves);
            break;

        case PieceType::King:
            generateJumpMoves(position, from, moves);
            generateCastlingMoves(position, from, CastlingSide::KingSide, moves);
            generateCastlingMoves(position, from, CastlingSide::QueenSide, moves);
            break;
        

        case PieceType::Bishop:
        case PieceType::Rook:
        case PieceType::Queen:
            generateSlidingMoves(position, from, moves);
            break;

        default: return;
    }   
}

