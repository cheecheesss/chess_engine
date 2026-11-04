#pragma once

#include <vector>
#include <cstdint>

#include "core/Position.hpp"
#include "core/Move.hpp"

class MoveGenerator {
    private:
        struct UndoInfo {
            Move move;
            Piece movedPiece;
            Piece capturedPiece;
            int capturedSquare{-1};
            int previousEnPassantTarget{-1};
        };

        bool isInsideBoard(int newX, int newY) const;
    
        std::vector<Move> generateJumpMoves(
            const Position& position,
            int from
        ) const; 
    
        std::vector<Move> generateSlidingMoves(
            const Position& position,
            int from
        ) const; 
    
        std::vector<Move> generatePawnMoves(
            const Position& position,
            int from
        ) const;
    
        bool canCastle(
            const Position& position,
            Color color, 
            CastlingSide side) const;

        std::vector<Move> generateCastlingMoves(
            const Position& position,
            int from,
            CastlingSide side
        ) const;
            
        
        UndoInfo makeMove(Position& position, const Move& move); 
        void undoMove(Position& position, const UndoInfo& undoInfo);
    
    public:
        std::vector<Move> generatePseudoLegalMoves(
            const Position& position,
            int from
        ) const;

        bool isSquareAttacked(
            const Position& position,
            int square,
            Color attacker
        ) const;

        bool isKingInCheck(
            const Position& position,
            Color kingColor
        ) const; 
    
};