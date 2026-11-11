#include "movegen/MoveGenerator.hpp"

bool MoveGenerator::isInsideBoard(int x, int y) const {
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

void MoveGenerator::generateJumpMoves(
    const Position& position,
    int from,
    MoveList& moves
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }
    Piece myPiece = position.at(from);
    PieceType figureType = myPiece.type;

    const int KnightMoves[8][2] = {
        {-1, 2}, {1, 2},
        {2, 1}, {-2, 1},
        {-2, -1}, {2, -1},
        {-1, -2}, {1, -2},
    }; 
    const int KingMoves[8][2] = {
        {1, -1}, {1, 1},
        {-1, -1}, {-1, 1},
        {1, 0}, {-1, 0},
        {0, 1}, {0, -1},
    };
    const int (*moveList)[2] = nullptr;

    if (figureType == PieceType::Knight) {
        moveList = KnightMoves;
    } else if (figureType == PieceType::King) {
        moveList = KingMoves;
    } else {
        throw std::logic_error("generateJumpMoves called with non-jumping piece type");
    }

    for (int i = 0; i < 8; ++i) {
        int newX = (from % 8) + moveList[i][0];
        int newY = (from / 8) + moveList[i][1];

        if (isInsideBoard(newX, newY)) {
            int to = newY * 8 + newX;
            Piece targetPiece = position.at(to);

            if (targetPiece.isEmpty() || targetPiece.color != myPiece.color) {
                moves.push_back(Move{from, to});
            }
        }
    }
}

void MoveGenerator::generateSlidingMoves(
    const Position& position,
    int from,
    MoveList& moves
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }
    Piece myPiece = position.at(from);
    PieceType figureType = myPiece.type;

    const int BishopMoves[4][2] = {
        {1, -1}, {1, 1},
        {-1, -1}, {-1, 1},
    };
    const int RookMoves[4][2] = {
        {1, 0}, {-1, 0},
        {0, 1}, {0, -1},
    };
    const int QueenMoves[8][2] = {
        {1, -1}, {1, 1},
        {-1, -1}, {-1, 1},
        {1, 0}, {-1, 0},
        {0, 1}, {0, -1},
    };

    const int (*moveList)[2] = nullptr;

    if (figureType == PieceType::Bishop) {
        moveList = BishopMoves;
    } else if (figureType == PieceType::Rook) {
        moveList = RookMoves;
    } else if (figureType == PieceType::Queen) {
        moveList = QueenMoves;
    } else {
        throw std::logic_error("generateSlidingMoves called with non-sliding piece type");
    }
    int directionCount = (figureType == PieceType::Queen) ? 8 : 4;

    for (int i = 0; i < directionCount; ++i) {
        int stepX = moveList[i][0];
        int stepY = moveList[i][1];

        int newX = (from % 8) + stepX;
        int newY = (from / 8) + stepY;

        while (isInsideBoard(newX, newY)) {
            int to = newY * 8 + newX;
            Piece targetPiece = position.at(to);

            if (targetPiece.isEmpty()) {
                moves.push_back(Move{from, to});
            } else if (targetPiece.color != myPiece.color) {
                moves.push_back(Move{from, to});
                break; // Stop sliding in this direction after capturing
            } else {
                break; // Blocked by own piece
            }

            newX += stepX;
            newY += stepY;
        }
    }
}

void MoveGenerator::generatePawnMoves(
    const Position& position,
    int from,
    MoveList& moves
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    Piece pawn = position.at(from);
    if (pawn.type != PieceType::Pawn) {
        throw std::logic_error("generatePawnMoves called with non-pawn piece type");
    }


    const int x = from % 8;
    const int y = from / 8;
    const int direction = pawn.color == Color::White ? -1 : 1;
    const int nextY = y + direction;
    const int promotionRank = pawn.color == Color::White ? 0 : 7;

    // лямбда фунция для добавления Ёходов пешки
    const auto addPawnMove = [&](int to) {
        if (to / 8 == promotionRank) {
            moves.push_back(Move{from, to, PieceType::Bishop});
            moves.push_back(Move{from, to, PieceType::Knight});
            moves.push_back(Move{from, to, PieceType::Queen});
            moves.push_back(Move{from, to, PieceType::Rook});
        } else {
            moves.push_back(Move{from, to});
        }
    };

    if (!isInsideBoard(x, nextY)) {
        return;
    }

    const int oneStepTo = nextY * 8 + x;
    if (position.at(oneStepTo).isEmpty()) {
        addPawnMove(oneStepTo);

        const bool isStartingRank =
            (pawn.color == Color::White && y == 6) ||
            (pawn.color == Color::Black && y == 1);

        const int doubleY = y + direction * 2;
        if (isStartingRank && isInsideBoard(x, doubleY)) {
            const int twoStepsTo = doubleY * 8 + x;
            if (position.at(twoStepsTo).isEmpty()) {
                moves.push_back(Move{from, twoStepsTo});
            }
        }
    }
    for (int dx : {-1, 1}) {
        const int captureX = x + dx;
        if (!isInsideBoard(captureX, nextY)) {
            continue;
        }

        const int captureTo = nextY * 8 + captureX;
        const Piece target = position.at(captureTo);
        if (!target.isEmpty() && target.color != pawn.color) {
            addPawnMove(captureTo);
        }
        else if (target.isEmpty() && captureTo == position.enPassantTarget()) {
            const int expectedTargetY = pawn.color == Color::White ? 2 : 5;
            const int capturedPawnSquare = captureTo - direction * 8;
            const Piece capturedPawn = position.at(capturedPawnSquare);

            if (nextY == expectedTargetY &&
                capturedPawn.type == PieceType::Pawn &&
                capturedPawn.color != pawn.color) {
                moves.push_back(Move{from, captureTo});
            }
        }
    }
}