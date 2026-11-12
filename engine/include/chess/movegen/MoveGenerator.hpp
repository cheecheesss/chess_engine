#pragma once

#include "core/Position.hpp"
#include "core/Move.hpp"

class MoveGenerator {
    private:
        /// @brief информация для вовзращения хода
        struct UndoInfo {
            Move move;
            Piece movedPiece;
            
            Piece capturedPiece;
            int capturedSquare{-1};

            int previousEnPassantTarget{-1};
            
            CastlingRights previousCastlingRights;
            int rookFrom{-1};
        };

        /// @brief проверка находится ли координата клетки по x и y в доске
        bool isInsideBoard(int x, int y) const;
    
        /// @brief генерирует ходы для коня и короля
        void generateJumpMoves(
            const Position& position,
            int from,
            MoveList& moves
        ) const; 
    
        /// @brief генерирует ходы для ладьи, слона и ферзя
        void generateSlidingMoves(
            const Position& position,
            int from,
            MoveList& moves
        ) const; 
    
        /// @brief генерирует ходы для пешки
        void generatePawnMoves(
            const Position& position,
            int from,
            MoveList& moves
        ) const;
    
        /// @brief проверка на возможность рокировки в определенную сторону
        bool canCastle(
            const Position& position,
            Color color, 
            CastlingSide side) const;
        
        /// @brief генерация рокировочных ходов
        void generateCastlingMoves(
            const Position& position,
            int from,
            CastlingSide side,
            MoveList& moves
        ) const;
            
        /// @brief сделать ход
        /// @return информацию для того что бы ход вернуть
        UndoInfo makeMove(Position& position, const Move& move) const; 
        /// @brief вернуть ход
        void undoMove(Position& position, const UndoInfo& undoInfo) const;
    
        /// @brief генерирует ходы без учета атаки на короля 
        void generatePseudoLegalMoves(
            const Position& position,
            int from,
            MoveList& pseudoMoves
        ) const;
    public:
        /// @brief генерирует полностью легальные ходы для стороны
        MoveList generateLegalMovesForSide(
            const Position& position,
            Color sideToMove
        ) const;

        /// @brief применяет ход из уже полученного списка
        bool applyLegalMove(
            Position& Position,
            const MoveList& legalMoves,
            const Move& requstedMove
        ) const;

        /// @brief проверка атакованна ли клетка
        bool isSquareAttacked(
            const Position& position,
            int square,
            Color attacker
        ) const;

        /// @brief проверка на шах
        bool isKingInCheck(
            const Position& position,
            Color kingColor
        ) const; 
    
};