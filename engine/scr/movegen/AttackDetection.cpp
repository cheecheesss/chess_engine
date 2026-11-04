#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

bool MoveGenerator::isSquareAttacked( // надо жестко оптимизировать
    const Position& position,
    int square,
    Color attacker
) const {
    // Проверяем, атакуется ли квадрат "square" фигурой цвета "attacker"
    for (int from = 0; from < 64; ++from) {
        Piece piece = position.at(from);
        if (!piece.isEmpty() && piece.color == attacker) {
            // сначала пешка
            if (piece.type == PieceType::Pawn) {
                const int direction = attacker == Color::White ? -1 : 1;
                const int x = from % 8;
                const int y = from / 8;
            
                for (int dx : {-1, 1}) {
                    if (x + dx == square % 8 &&
                        y + direction == square / 8) {
                        return true;
                    }
                }   
                continue;
            }
            // если не пешка то все остальное
            std::vector<Move> moves = piece.type == PieceType::King
                ? generateJumpMoves(position, from)
                : generatePseudoLegalMoves(position, from);
            for (const Move& move : moves) {
                if (move.to == square) {
                    return true; // Квадрат атакуется
                }
            }
        }
    }
    return false; // Квадрат не атакуется
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