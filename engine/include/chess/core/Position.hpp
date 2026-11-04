#pragma once

#include <array>
#include "Types.hpp"

struct CastlingRights {
    bool whiteKingSide{false};
    bool whiteQueenSide{false};
    bool blackKingSide{false};
    bool blackQueenSide{false};
};
class Position {
public:
    Position() = default; // because we create board on private 
    Piece at(int square) const;
    void setPiece(int square, Piece piece);

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