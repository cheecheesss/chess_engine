#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

bool MoveGenerator::isSquareAttacked( // надо жестко оптимизировать
    const Position& position,
    int square,
    Color attacker
) const {
    if (square < 0 || square >= 64) {
        return false;
    }

    constexpr int knightOffsets[8][2] = {
        {-1, 2}, {1, 2}, {2, 1}, {-2, 1},
        {-2, -1}, {2, -1}, {-1, -2}, {1, -2}
    };
    constexpr int directions[8][2] = {
        {1, -1}, {1, 1}, {-1, -1}, {-1, 1},
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    const int targetX = square % 8;
    const int targetY = square / 8;

    const int pawnSourceY = targetY + (attacker == Color::White ? 1 : -1);
    if (pawnSourceY >= 0 && pawnSourceY < 8) {
        for (int dx : {-1, 1}) {
            const int sourceX = targetX + dx;
            if (sourceX < 0 || sourceX >= 8) {
                continue;
            }

            const Piece piece = position.at(pawnSourceY * 8 + sourceX);
            if (piece.type == PieceType::Pawn && piece.color == attacker) {
                return true;
            }
        }
    }

    for (const auto& offset : knightOffsets) {
        const int x = targetX + offset[0];
        const int y = targetY + offset[1];
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            const Piece piece = position.at(y * 8 + x);
            if (piece.type == PieceType::Knight && piece.color == attacker) {
                return true;
            }
        }
    }

    for (const auto& direction : directions) {
        const int stepX = direction[0];
        const int stepY = direction[1];
        int x = targetX + stepX;
        int y = targetY + stepY;
        int distance = 1;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            const Piece piece = position.at(y * 8 + x);
            if (!piece.isEmpty()) {
                if (piece.color == attacker) {
                    const bool diagonal = stepX != 0 && stepY != 0;
                    if (piece.type == PieceType::King && distance == 1) {
                        return true;
                    }
                    if (piece.type == PieceType::Queen ||
                        (diagonal && piece.type == PieceType::Bishop) ||
                        (!diagonal && piece.type == PieceType::Rook)) {
                        return true;
                    }
                }
                break;
            }
            x += stepX;
            y += stepY;
            ++distance;
        }
    }
    return false;
}
/// @brief Проверяет, находится ли король под шахом
/// @param position 
/// @param kingColor 
/// @return bool - true если король под шахом, false иначе
bool MoveGenerator::isKingInCheck(
    const Position& position,
    Color kingColor
) const {
    //ищем короля на доске
    int kingSquare = -1;
    for (int square = 0; square < 64; ++square) {
        Piece piece = position.at(square);
        if (!piece.isEmpty() && piece.type == PieceType::King && piece.color == kingColor) {
            kingSquare = square;
            break;
        }
    }
    if (kingSquare == -1) {
        throw std::logic_error("King not found on the board"); // Король не найден
    }
    Color opponentColor = (kingColor == Color::White) ? Color::Black : Color::White;
    return isSquareAttacked(position, kingSquare, opponentColor);
}