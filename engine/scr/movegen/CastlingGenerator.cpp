#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

bool MoveGenerator::canCastle(
    const Position& position,
    Color color,
    CastlingSide side
) const {
    if (!position.castlingRight(color, side)) {
        return false;
    }
    const int kingSquare = color == Color::White ? 60 : 4;
    const int rookSquare = side == CastlingSide::KingSide
        ? (color == Color::White ? 63 : 7)
        : (color == Color::White ? 56 : 0);

    const Piece king = position.at(kingSquare);
    const Piece rook = position.at(rookSquare);
    if (king.type != PieceType::King || king.color != color ||
        rook.type != PieceType::Rook || rook.color != color) {
        return false;
    }

    if (isKingInCheck(position, color)) {
        return false;
    }

    const int step = side == CastlingSide::KingSide ? 1 : -1;
    const Color attacker = color == Color::White ? Color::Black : Color::White;
    for (int i = 1; i <= 2; ++i) {
        const int square = kingSquare + step * i;
        if (!position.at(square).isEmpty() ||
            isSquareAttacked(position, square, attacker)) {
            return false;
        }
    }

    if (side == CastlingSide::QueenSide &&
        !position.at(kingSquare - 3).isEmpty()) {
        return false;
    }

    return true;
}

std::vector<Move> MoveGenerator::generateCastlingMoves(
    const Position& position,
    int from,
    CastlingSide side
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    const Piece king = position.at(from);
    if (king.type != PieceType::King) {
        return {};
    }

    const int homeSquare = king.color == Color::White ? 60 : 4;
    if (from != homeSquare || !canCastle(position, king.color, side)) {
        return {};
    }

    const int to = side == CastlingSide::KingSide ? from + 2 : from - 2;
    return {Move{from, to}};
}
