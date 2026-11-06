#pragma once

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
            MoveList& moves
        ) const; 
    
        void generateSlidingMoves(
            const Position& position,
            int from,
            MoveList& moves
        ) const; 
    
        void generatePawnMoves(
            const Position& position,
            int from,
            MoveList& moves
        ) const;
    
        bool canCastle(
            const Position& position,
            Color color, 
            CastlingSide side) const;

        void generateCastlingMoves(
            const Position& position,
            int from,
            CastlingSide side,
            MoveList& moves
        ) const;
            
        
        UndoInfo makeMove(Position& position, const Move& move) const; 
        void undoMove(Position& position, const UndoInfo& undoInfo) const;
    
        void generatePseudoLegalMoves(
            const Position& position,
            int from,
            MoveList& pseudoMoves
        ) const;
    public:

        MoveList generateLegalMovesForSide(
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