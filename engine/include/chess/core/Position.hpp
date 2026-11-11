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
/// @brief хранит состояние шахматной позиции (как array<Piece, 64>)
class Position {
public:
    Position() = default;

    /// @return фигуру на доске
    Piece at(int square) const {
        if (square < 0 || square >= 64) {
            throw std::out_of_range("Invalid square");
        }
        return board_[static_cast<std::size_t>(square)];
    }
    /// @brief ставит фигуру на указанную клетку
    void setPiece(int square, Piece piece) {
        if (square < 0 || square >= 64) {
            throw std::out_of_range("Invalid square");
        }
        board_[static_cast<std::size_t>(square)] = piece;
    }

    /// @return клетку атакованную взятием на проходе
    int enPassantTarget() const;
    /// @brief устанавливает атакованную взятием на проходе клетку
    void setEnPassantTarget(int square);

    /// @return все права на рокировку
    CastlingRights getCastlingRights() const;

    /// @return право на определенную рокировку
    bool castlingRight(
        Color color, 
        CastlingSide side
    ) const;
    
    /// @brief меняет право на определенную рокировку
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