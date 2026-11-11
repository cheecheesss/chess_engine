#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include "Types.hpp"


/// @brief движение от клетки в клетку. 
/// номера клеток должны быть в диапазоне от 0 до 64
struct Move {
    std::uint8_t from;
    std::uint8_t to;
    PieceType promotion{PieceType::None};
    constexpr Move() noexcept 
        : from(0), to(0), promotion(PieceType::None) {}

    constexpr Move(
        int fromSquare,
        int toSquare,
        PieceType promotionType = PieceType::None
    ) noexcept
        : from(static_cast<std::uint8_t>(fromSquare)),
          to(static_cast<std::uint8_t>(toSquare)),
          promotion(promotionType) {
        assert(fromSquare >= 0 && fromSquare < 64);
        assert(toSquare >= 0 && toSquare < 64);
    }
};

/// @brief свой класс заменяющий std::array в целях
/// оптимизации. Имеет выделенную память - 256 и ограниченный функционал.
class MoveList {
public:
    static constexpr std::size_t Capacity = 256;

    using iterator = Move*;
    using const_iterator = const Move*;

    void clear() noexcept {
        size_ = 0;
    }
    void push_back(const Move& move) {
        if (size_ == Capacity) {
            throw std::length_error("MoveList capacity exceeded");
        }
        moves_[size_++] = move;
    }
    std::size_t size() const noexcept {
        return size_;
    }
    bool empty() const noexcept {
        return size_ == 0;
    }
    iterator begin() noexcept {
        return moves_.data();
    }
    iterator end() noexcept {
        return moves_.data() + size_;
    }
    const_iterator begin() const noexcept {
        return moves_.data();
    }
    const_iterator end() const noexcept {
        return moves_.data() + size_;
    }

private:
    std::array<Move, Capacity> moves_{};
    std::uint16_t size_{0};
};
