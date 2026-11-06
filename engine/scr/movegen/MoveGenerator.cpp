#include "movegen/MoveGenerator.hpp"

/// @brief Генерирует псевдо-законные ходы для фигуры на указанной позиции
/// @param position 
/// @param from
/// @param moves
void MoveGenerator::generatePseudoLegalMoves(
    const Position& position, // list 0-63 (squares of board)
    int from, // move from coord
    MoveList& moves
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

        case PieceType::King: {
            generateJumpMoves(position, from, moves);
            const int homeSquare = piece.color == Color::White ? 60 : 4;

            const bool hasCastlingRight =
                position.castlingRight(piece.color, CastlingSide::KingSide) ||
                position.castlingRight(piece.color, CastlingSide::QueenSide);

            if (from == homeSquare &&
                hasCastlingRight &&
                !isKingInCheck(position, piece.color)) {
                generateCastlingMoves(position, from, CastlingSide::KingSide, moves);
                generateCastlingMoves(position, from, CastlingSide::QueenSide, moves);
            }
            break;
        }

        case PieceType::Bishop:
        case PieceType::Rook:
        case PieceType::Queen:
            generateSlidingMoves(position, from, moves);
            break;

        default:
            return;
    }   
}

