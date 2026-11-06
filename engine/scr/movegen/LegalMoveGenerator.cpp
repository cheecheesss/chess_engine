#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

std::vector<Move> MoveGenerator::generateLegalMovesForSide(
    const Position& position,
    Color sideToMove
) const {
    std::vector<Move> legalMoves;
    // ========= НАХОЖДЕНИЕ КОРОЛЯ ==========
    int kingSquare = -1;
    for (int square = 0; square < 64; ++square) {
        const Piece piece = position.at(square);
        if (piece.type == PieceType::King && piece.color == sideToMove) {
            kingSquare = square;
            break;
        }
    }
    if (kingSquare == -1) {
        throw std::logic_error("King not found on the board");
    }
    const Color opponentColor =
    sideToMove == Color::White ? Color::Black : Color::White;
    
    // ========= ГЕНЕРАЦИЯ ХОДОВ ==========
    Position tempPosition = position;
    std::vector<Move> pseudoMoves;
    pseudoMoves.reserve(32);
    for (int square = 0; square < 64; ++square) {
        Piece piece = position.at(square);
        if (piece.isEmpty() || piece.color != sideToMove) {
            continue;
        }
        pseudoMoves.clear();
        generatePseudoLegalMoves(position, square, pseudoMoves);
        
        // проверяем каждый ход на предмет того, не оставляет ли он короля под шахом    
        for (const Move& move : pseudoMoves) {
            UndoInfo undoInfo = makeMove(tempPosition, move);
            
            const int kingSquareAfterMove =
                piece.type == PieceType::King ? move.to : kingSquare;

            if (!isSquareAttacked(
                    tempPosition, kingSquareAfterMove, opponentColor)) {
                legalMoves.push_back(move);
            }
            undoMove(tempPosition, undoInfo);
        }
    }
    return legalMoves;
}