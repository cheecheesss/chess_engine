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
            
            CastlingRights previousCastlingRights;
            int rookFrom{-1};
        };

        bool isInsideBoard(int newX, int newY) const;
    
        void generateJumpMoves(
            const Position& position,
            int from,
            std::vector<Move>& moves
        ) const; 
    
        void generateSlidingMoves(
            const Position& position,
            int from,
            std::vector<Move>& moves
        ) const; 
    
        void generatePawnMoves(
            const Position& position,
            int from,
            std::vector<Move>& moves
        ) const;
    
        bool canCastle(
            const Position& position,
            Color color, 
            CastlingSide side) const;

        void generateCastlingMoves(
            const Position& position,
            int from,
            CastlingSide side,
            std::vector<Move>& moves
        ) const;
            
        
        UndoInfo makeMove(Position& position, const Move& move) const; 
        void undoMove(Position& position, const UndoInfo& undoInfo) const;
    
        void generatePseudoLegalMoves(
            const Position& position,
            int from,
            std::vector<Move>& pseudoMoves
        ) const;
    public:

        std::vector<Move> generateLegalMovesForSide(
            const Position& position,
            Color sideToMove
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