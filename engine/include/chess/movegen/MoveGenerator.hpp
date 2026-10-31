#pragma once

#include <vector>

#include "core/Position.hpp"
#include "core/Move.hpp"

class MoveGenerator {
public:
    std::vector<Move> generatePseudoLegalMoves(
        const Position& position,
        int from
    ) const;
    bool isSquareAttacked() const;
    bool isKingInCheck() const; 
private:
    std::vector<Move> generateJumpMoves() const; 
    std::vector<Move> generateSlidingMoves() const;
    
    bool isInsideBoard(int newX, int newY) const;
    void makeMove() const; 
    void undoMove() const;
};
