#include <stdexcept>
#include "Position.hpp"

Piece Position::at(int square) const {
    if (square < 0 || square >= 64) {
        throw std::out_of_range("Invalid square");
    } 
    return board_[square];
}

void Position::setPiece(int square, Piece piece) {
    if (square < 0 || square >= 64) {
        throw std::out_of_range("Invalid square");
    } 
    board_[square] = piece;
}