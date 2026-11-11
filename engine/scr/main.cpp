#include "movegen/MoveGenerator.hpp"

int main() {
    MoveGenerator moveGenerator;
    Position position;
    Color sideToMove;

    // игровой цикл
    while (true) {

        // проверка на окончание игры
        MoveList moves =  moveGenerator.generateLegalMovesForSide(position, sideToMove);
        if (moves.empty()) {
            if (moveGenerator.isKingInCheck(position, sideToMove)) {
                break; // мат
            } else {
                break; // пат
            }
        }
    }
}