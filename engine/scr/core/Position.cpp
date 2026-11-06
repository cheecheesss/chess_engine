#include "Position.hpp"

int Position::enPassantTarget() const {
    return enPassantTarget_;
}

void Position::setEnPassantTarget(int square) {
    if (square < -1 || square >= 64) {
        throw std::out_of_range("Invalid en passant target");
    }
    if (square != -1 && square / 8 != 2 && square / 8 != 5) {
        throw std::invalid_argument("En passant target must be on rank 3 or 6");
    }
    enPassantTarget_ = square;
}
CastlingRights Position::getCastlingRights() const {
    return castlingRights_;
}

bool Position::castlingRight(
    Color color, 
    CastlingSide side
) const {
    if (color == Color::White) {
        return side == CastlingSide::KingSide
            ? castlingRights_.whiteKingSide
            : castlingRights_.whiteQueenSide;
    } else {
        return side == CastlingSide::KingSide
            ? castlingRights_.blackKingSide
            : castlingRights_.blackQueenSide;
    }
}

void Position::setCastlingRight(
    Color color, 
    CastlingSide side,
    bool allowed
) {
    if (color == Color::White) {
        if (side == CastlingSide::KingSide) {
            castlingRights_.whiteKingSide = allowed;
        } else {
            castlingRights_.whiteQueenSide = allowed;
        }
    } else {
        if (side == CastlingSide::KingSide) {
            castlingRights_.blackKingSide = allowed;
        } else {
            castlingRights_.blackQueenSide = allowed;
        }
    }
}
