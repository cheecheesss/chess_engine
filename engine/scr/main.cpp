#include <iostream>
#include <limits>
#include <string>
#include "core/Position.hpp"
#include "movegen/MoveGenerator.hpp"

Position generateStartPosition() {
    Position position;
    constexpr PieceType backRank[8] = {
        PieceType::Rook,
        PieceType::Knight,
        PieceType::Bishop,
        PieceType::Queen,
        PieceType::King,
        PieceType::Bishop,
        PieceType::Knight,
        PieceType::Rook
    };

    for (int file = 0; file < 8; ++file) {
        position.setPiece(file, Piece{backRank[file], Color::Black});
        position.setPiece(8 + file, Piece{PieceType::Pawn, Color::Black});
        position.setPiece(48 + file, Piece{PieceType::Pawn, Color::White});
        position.setPiece(56 + file, Piece{backRank[file], Color::White});
    }

    position.setCastlingRight(Color::White, CastlingSide::KingSide, true);
    position.setCastlingRight(Color::White, CastlingSide::QueenSide, true);
    position.setCastlingRight(Color::Black, CastlingSide::KingSide, true);
    position.setCastlingRight(Color::Black, CastlingSide::QueenSide, true);
    return position;
}

char pieceSymbol(Piece piece) {
    if (piece.isEmpty()) {
        return '.';
    }

    char symbol = '?';
    switch (piece.type) {
        case PieceType::Pawn:   symbol = 'P'; break;
        case PieceType::Knight: symbol = 'N'; break;
        case PieceType::Bishop: symbol = 'B'; break;
        case PieceType::Rook:   symbol = 'R'; break;
        case PieceType::Queen:  symbol = 'Q'; break;
        case PieceType::King:   symbol = 'K'; break;
        case PieceType::None:   return '.';
    }

    if (piece.color == Color::Black) {
        return static_cast<char>(symbol - 'A' + 'a');
    }
    return symbol;
}

void printPosition(
    const Position& position,
    Color color,
    const char* status = "ongoing"
) {
    std::cout << "board ";
    for (int square = 0; square < 64; ++square) {
        std::cout << pieceSymbol(position.at(square));
    }
    std::cout << ' ' << (color == Color::White ? 'w' : 'b')
              << ' ' << status << '\n' << std::flush;
}

int main() {
    MoveGenerator moveGenerator;
    Position position = generateStartPosition();
    Color sideToMove = Color::White;
    printPosition(position, sideToMove);

    while (true) {
        MoveList moves =  moveGenerator.generateLegalMovesForSide(position, sideToMove);

        // ============== ПРОВЕРКА ОКОНЧАНИЯ ==============
        if (moves.empty()) {
            if (moveGenerator.isKingInCheck(position, sideToMove)) {
                printPosition(position, sideToMove, "checkmate");
                break;
            } else {
                printPosition(position, sideToMove, "stalemate");
                break; // пат
            }
        }
        // ============== ПОЛУЧАЕМ ХОД ==============
        int from, to;
        if (!(std::cin >> from >> to)) {
            if (std::cin.eof()) {
                break; // Python закрыл ввод или пользователь нажал Ctrl+Z
            }
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
            std::cout << "error bad_move\n" << std::flush;
            continue;
        }

        if (from < 0 || from >= 64 || to < 0 || to >= 64) {
            std::cout << "error bad_move\n" << std::flush;
            continue;
        }

        // ============== ПРИМЕНЯЕМ ХОД ==============
        const Move requestedMove{from, to};
        if (!moveGenerator.applyLegalMove(position, moves, requestedMove)) {
            std::cout << "error illegal_move\n" << std::flush;
            continue;
        }
        sideToMove = sideToMove == Color::White
            ? Color::Black
            : Color::White;

        printPosition(position, sideToMove);
    }
    return 0;
}