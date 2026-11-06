#pragma once

#include <array>
#include <stdexcept>
#include "Types.hpp"

struct CastlingRights {
    bool whiteKingSide{false};
    bool whiteQueenSide{false};
    bool blackKingSide{false};
    bool blackQueenSide{false};
};
class Position {
public:
    Position() = default;

    Piece at(int square) const {
        if (square < 0 || square >= 64) {
            throw std::out_of_range("Invalid square");
        }
        return board_[static_cast<std::size_t>(square)];
    }

    void setPiece(int square, Piece piece) {
        if (square < 0 || square >= 64) {
            throw std::out_of_range("Invalid square");
        }
        board_[static_cast<std::size_t>(square)] = piece;
    }

    int enPassantTarget() const;
    void setEnPassantTarget(int square);

    CastlingRights getCastlingRights() const;
    bool castlingRight(
        Color color, 
        CastlingSide side
    ) const;
    void setCastlingRight(
        Color color, 
        CastlingSide side,
        bool allowed
    );

private:
    std::array<Piece, 64> board_{};
    int enPassantTarget_{-1};
    CastlingRights castlingRights_{};
};