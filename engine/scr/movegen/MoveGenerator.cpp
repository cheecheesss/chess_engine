#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

bool MoveGenerator::isInsideBoard(int x, int y) const {
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

std::vector<Move> MoveGenerator::generateJumpMoves(
    const Position& position,
    int from
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }
    std::vector<Move> moves;
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
    return moves;
}

std::vector<Move> MoveGenerator::generateSlidingMoves(
    const Position& position,
    int from
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }
    std::vector<Move> moves;
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
    return moves;
}

std::vector<Move> MoveGenerator::generatePawnMoves(
    const Position& position,
    int from
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    Piece pawn = position.at(from);
    if (pawn.type != PieceType::Pawn) {
        throw std::logic_error("generatePawnMoves called with non-pawn piece type");
    }

    std::vector<Move> moves;
    const int x = from % 8;
    const int y = from / 8;
    const int direction = pawn.color == Color::White ? -1 : 1;
    const int nextY = y + direction;

    if (!isInsideBoard(x, nextY)) {
        return moves;
    }

    const int oneStepTo = nextY * 8 + x;
    if (position.at(oneStepTo).isEmpty()) {
        moves.push_back(Move{from, oneStepTo});

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
            moves.push_back(Move{from, captureTo});
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
    return moves;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(
    const Position& position, // list 0-63 (squares of board)
    int from // move from coord
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    Piece piece = position.at(from);
    if (piece.isEmpty()) {
        return {};
    }

    switch (piece.type) {
        case PieceType::Pawn:
            return generatePawnMoves(position, from);

        case PieceType::Knight:
            return generateJumpMoves(position, from);

        case PieceType::King: {
            std::vector<Move> moves = generateJumpMoves(position, from);
            for (CastlingSide side : {CastlingSide::KingSide, CastlingSide::QueenSide}) {
                std::vector<Move> castlingMoves =
                    generateCastlingMoves(position, from, side);
                moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());
            }
            return moves;
        }

        case PieceType::Bishop:
        case PieceType::Rook:
        case PieceType::Queen:
            return generateSlidingMoves(position, from);
        
        default: return {};
    }   
}

bool MoveGenerator::isSquareAttacked( // пока что не работает нормально с пешками
    const Position& position,
    int square,
    Color attacker
) const {
    // Проверяем, атакуется ли квадрат "square" фигурой цвета "attacker"
    for (int from = 0; from < 64; ++from) {
        Piece piece = position.at(from);
        if (!piece.isEmpty() && piece.color == attacker) {
            // сначала пешка
            if (piece.type == PieceType::Pawn) {
                const int direction = attacker == Color::White ? -1 : 1;
                const int x = from % 8;
                const int y = from / 8;
            
                for (int dx : {-1, 1}) {
                    if (x + dx == square % 8 &&
                        y + direction == square / 8) {
                        return true;
                    }
                }
                continue;
            }
            // если не пешка то все остальное
            std::vector<Move> moves = piece.type == PieceType::King
                ? generateJumpMoves(position, from)
                : generatePseudoLegalMoves(position, from);
            for (const Move& move : moves) {
                if (move.to == square) {
                    return true; // Квадрат атакуется
                }
            }
        }
    }
    return false; // Квадрат не атакуется
}

bool MoveGenerator::isKingInCheck(
    const Position& position,
    Color kingColor
) const {
    //ищем короля на доске
    int kingSquare = -1;
    for (int square = 0; square < 64; ++square) {
        Piece piece = position.at(square);
        if (!piece.isEmpty() && piece.type == PieceType::King && piece.color == kingColor) {
            kingSquare = square;
            break;
        }
    }
    if (kingSquare == -1) {
        return false; // Король не найден
    }
    Color opponentColor = (kingColor == Color::White) ? Color::Black : Color::White;
    return isSquareAttacked(position, kingSquare, opponentColor);
}

bool MoveGenerator::canCastle(
    const Position& position,
    Color color,
    CastlingSide side
) const {
    const int kingSquare = color == Color::White ? 60 : 4;
    const int rookSquare = side == CastlingSide::KingSide
        ? (color == Color::White ? 63 : 7)
        : (color == Color::White ? 56 : 0);

    const Piece king = position.at(kingSquare);
    const Piece rook = position.at(rookSquare);
    if (king.type != PieceType::King || king.color != color ||
        rook.type != PieceType::Rook || rook.color != color) {
        return false;
    }

    if (isKingInCheck(position, color)) {
        return false;
    }

    const int step = side == CastlingSide::KingSide ? 1 : -1;
    const Color attacker = color == Color::White ? Color::Black : Color::White;
    for (int i = 1; i <= 2; ++i) {
        const int square = kingSquare + step * i;
        if (!position.at(square).isEmpty() ||
            isSquareAttacked(position, square, attacker)) {
            return false;
        }
    }

    if (side == CastlingSide::QueenSide &&
        !position.at(kingSquare - 3).isEmpty()) {
        return false;
    }

    return true;
}

std::vector<Move> MoveGenerator::generateCastlingMoves(
    const Position& position,
    int from,
    CastlingSide side
) const {
    if (from < 0 || from >= 64) {
        throw std::invalid_argument("Invalid square index");
    }

    const Piece king = position.at(from);
    if (king.type != PieceType::King) {
        return {};
    }

    const int homeSquare = king.color == Color::White ? 60 : 4;
    if (from != homeSquare || !canCastle(position, king.color, side)) {
        return {};
    }

    const int to = side == CastlingSide::KingSide ? from + 2 : from - 2;
    return {Move{from, to}};
}
