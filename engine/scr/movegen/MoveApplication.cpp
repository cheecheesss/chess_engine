#include <stdexcept>
#include "movegen/MoveGenerator.hpp"

/// @brief Применяет ход и возвращает данные для его отмены.
/// @param position Позиция, которую нужно изменить.
/// @param move Ход, который нужно применить.
/// @return Данные, необходимые для отмены этого хода.
MoveGenerator::UndoInfo MoveGenerator::makeMove(
    Position& position,
    const Move& move
) const {
    // Проверяем, что исходное поле занято фигурой.
    const Piece movedPiece = position.at(move.from);

    // ========== ВЗЯТИЕ НА ПРОХОДЕ ==========
    // Не допускаем взятие своей фигуры.
    const int previousEnPassantTarget = position.enPassantTarget();
    const Piece targetPiece = position.at(move.to);
    if (!targetPiece.isEmpty() && targetPiece.color == movedPiece.color) {
        throw std::invalid_argument("Cannot capture own piece");
    }

    // Вычисляем направление и смещение хода.
    const int direction = movedPiece.color == Color::White ? -1 : 1;
    const int fileDifference = move.to % 8 - move.from % 8;
    const int rankDifference = move.to / 8 - move.from / 8;

    Piece capturedPiece = targetPiece;
    int capturedSquare = targetPiece.isEmpty() ? -1 : move.to;

    // Проверка на взятие на проходе
    const bool isEnPassant =
        movedPiece.type == PieceType::Pawn &&
        targetPiece.isEmpty() &&
        (fileDifference == -1 || fileDifference == 1) &&
        move.to == previousEnPassantTarget;

    // Пешка не может ходить по диагонали на пустое поле, кроме en passant.
    if (movedPiece.type == PieceType::Pawn &&
        targetPiece.isEmpty() &&
        (fileDifference == -1 || fileDifference == 1) &&
        !isEnPassant) {
        throw std::invalid_argument("Diagonal pawn move requires a capture");
    }

    // Проверяем корректность взятия на проходе.
    if (isEnPassant) {
        const int expectedTargetRank =
            movedPiece.color == Color::White ? 2 : 5;

        if (rankDifference != direction ||
            move.to / 8 != expectedTargetRank) {
            throw std::invalid_argument("Invalid en passant move");
        }

        capturedSquare = move.to - direction * 8;
        capturedPiece = position.at(capturedSquare);

        if (capturedPiece.type != PieceType::Pawn ||
            capturedPiece.color == movedPiece.color) {
            throw std::invalid_argument("No enemy pawn to capture en passant");
        }
    }

    // Проверяем двойной ход пешки со стартовой горизонтали.
    int newEnPassantTarget = -1;
    if (movedPiece.type == PieceType::Pawn &&
        fileDifference == 0 &&
        rankDifference == direction * 2) {
        const int startingRank =
            movedPiece.color == Color::White ? 6 : 1;
        const int middleSquare = (move.from + move.to) / 2;

        if (move.from / 8 != startingRank ||
            !targetPiece.isEmpty() ||
            !position.at(middleSquare).isEmpty()) {
            throw std::invalid_argument("Invalid pawn double move");
        }

        newEnPassantTarget = middleSquare;
    }

    // ========== РОКИРОВКА ==========
    CastlingRights previousCastlingRights = position.getCastlingRights();

    int rookFrom = -1;
    int rookTo = -1;
    const bool isCastlingMove =
        movedPiece.type == PieceType::King &&
        (fileDifference == 2 || fileDifference == -2);

    if (isCastlingMove) {
        if (fileDifference == 2) { // короткая
            rookFrom = move.from + 3;
            rookTo = move.from + 1;
        } else { // длинная
            rookFrom = move.from - 4;
            rookTo = move.from - 1;
        }
    }
    if (capturedPiece.type == PieceType::Rook) {
        const Color rookColor = capturedPiece.color;

        if (capturedSquare == (rookColor == Color::White ? 63 : 7)) {
            position.setCastlingRight(
                rookColor, CastlingSide::KingSide, false);
        } else if (capturedSquare == (rookColor == Color::White ? 56 : 0)) {
            position.setCastlingRight(
                rookColor, CastlingSide::QueenSide, false);
        }
    }

    // Обновляем права на рокировку, если король или ладья были перемещены.
    if (movedPiece.type == PieceType::King) {
        position.setCastlingRight(movedPiece.color, CastlingSide::KingSide, false);
        position.setCastlingRight(movedPiece.color, CastlingSide::QueenSide, false);
    }
    if (movedPiece.type == PieceType::Rook) {
        if (move.from == (movedPiece.color == Color::White ? 63 : 7)) {
            position.setCastlingRight(movedPiece.color, CastlingSide::KingSide, false);
        } else if (move.from == (movedPiece.color == Color::White ? 56 : 0)) {
            position.setCastlingRight(movedPiece.color, CastlingSide::QueenSide, false);
        }
    }

    // ========== ОБНОВЛЕНИЕ ПОЗИЦИИ ==========
    position.setPiece(move.from, Piece{});
    if (capturedSquare != -1) {
        position.setPiece(capturedSquare, Piece{});
    }
    if (isCastlingMove) {
        const Piece rook = position.at(rookFrom);
        position.setPiece(rookFrom, Piece{});
        position.setPiece(rookTo, rook);
    }
    position.setPiece(move.to, movedPiece);
    position.setEnPassantTarget(newEnPassantTarget);

    return UndoInfo{
        move,
        movedPiece,
        capturedPiece,
        capturedSquare,
        previousEnPassantTarget,
        previousCastlingRights,
        rookFrom
    };
}

/// @brief Отменяет ход, используя сохранённые данные.
/// @param position Позиция, которую нужно восстановить.
/// @param undoInfo Сохранённые данные для отмены хода.
void MoveGenerator::undoMove(
    Position& position,
    const MoveGenerator::UndoInfo& undoInfo
) const {
    position.setPiece(undoInfo.move.to, Piece{});
    position.setPiece(undoInfo.move.from, undoInfo.movedPiece);

    if (undoInfo.capturedSquare != -1) {
        position.setPiece(undoInfo.capturedSquare, undoInfo.capturedPiece);
    }
    position.setEnPassantTarget(undoInfo.previousEnPassantTarget);

    if (undoInfo.rookFrom != -1) {
        const int rookTo = undoInfo.move.to > undoInfo.move.from
        ? undoInfo.move.from + 1
        : undoInfo.move.from - 1;
        
        const Piece rook = position.at(rookTo);
        position.setPiece(rookTo, Piece{});
        position.setPiece(undoInfo.rookFrom, rook);
    }
    position.setCastlingRight(Color::White, CastlingSide::KingSide, undoInfo.previousCastlingRights.whiteKingSide);
    position.setCastlingRight(Color::White, CastlingSide::QueenSide, undoInfo.previousCastlingRights.whiteQueenSide);
    position.setCastlingRight(Color::Black, CastlingSide::KingSide, undoInfo.previousCastlingRights.blackKingSide);
    position.setCastlingRight(Color::Black, CastlingSide::QueenSide, undoInfo.previousCastlingRights.blackQueenSide);
}