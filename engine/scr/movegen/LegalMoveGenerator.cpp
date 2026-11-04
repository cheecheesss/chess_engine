#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

std::vector<Move> MoveGenerator::generateLegalMovesForSide(
    const Position& position,
    Color sideToMove
) const {
    std::vector<Move> legalMoves;

    for (int square = 0; square < 64; ++square) {
        Piece piece = position.at(square);
        if (piece.isEmpty() || piece.color != sideToMove) {
            continue;
        }

        if (!piece.isEmpty()) {
            std::vector<Move> pseudoLegalMoves =
                generatePseudoLegalMoves(position, square);
            for (const Move& move : pseudoLegalMoves) {
                Position tempPosition = position;
                UndoInfo undoInfo = makeMove(tempPosition, move);
                
                if (!isKingInCheck(tempPosition, piece.color)) {
                    legalMoves.push_back(move);
                }
                undoMove(tempPosition, undoInfo);
            }
        }
    }
    return legalMoves;
}