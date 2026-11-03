#pragma once

#include <array>
#include "Types.hpp"

class Position {
public:
    Position() = default; // because we create board on private 

    Piece at(int square) const;
    void setPiece(int square, Piece piece);
    int enPassantTarget() const;
    void setEnPassantTarget(int square);

private:
    std::array<Piece, 64> board_{};
    int enPassantTarget_{-1};
};