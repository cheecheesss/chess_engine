#pragma once

#include <cstdint>

enum class Color : uint8_t {
    // 8 bit is enough
    White,
    Black
};

enum class PieceType : uint8_t {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

struct Piece {
    PieceType type{PieceType::None};
    Color color{Color::White};

    [[nodiscard]] bool isEmpty() const {
        return type == PieceType::None;
    }
};