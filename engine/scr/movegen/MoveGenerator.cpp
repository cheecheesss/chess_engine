#include "movegen/MoveGenerator.hpp"

// ideas 
// generateJumpMoves() — для коня и короля.
// generateSlidingMoves() — для слона, ладьи и ферзя.

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(
    const Position& position, // list 0-63 (squares of board)
    int from // move from coord
) const {
    std::vector<Move> moves; 
    
    int x = from  % 8;
    int y = from / 8;
    
    Piece myPiece = position.at(from);
    if (myPiece.isEmpty()) return moves;

    switch (myPiece.type) {
        case PieceType::Knight: {
            // knight search move mechanic 
            const int KnightMoves[8][2] = {
                // {x, y}
                {-1, 2}, {1, 2},
                {2, 1}, {-2, 1},
                {-2, -1}, {2, -1},
                {-1, -2}, {1, -2},
            }; 
            for (int i = 0; i < 8; ++i){
                int newX = x + KnightMoves[i][0];
                int newY = y + KnightMoves[i][1];
        
                // В ПОЛЕ МОЖНО!
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    int to = newY * 8 + newX;
                    Piece targetPiece = position.at(to);
                    
                    // ПУСТО ИЛИ ФИГУРА МОЖНО !
                    if (targetPiece.isEmpty() || targetPiece.color != myPiece.color)
                        moves.push_back(Move(from, to));
                }
            }
            break;
        }
        
        case PieceType::King: {
            // king search move mechanic 
            const int KingMoves[8][2] = {
                {1, -1}, {1, 1},
                {-1, -1}, {-1, 1},
                {1, 0}, {-1, 0},
                {0, 1}, {0, -1},
            };
            for (int i = 0; i < 8; ++i) {
                int newX = x + KingMoves[i][0];
                int newY = y + KingMoves[i][1];
                
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    int to = newY * 8 + newX;
                    Piece targetPiece = position.at(to);
                    
                    // ПУСТО МОЖНО!
                    if (targetPiece.isEmpty() || targetPiece.color != myPiece.color)
                        moves.push_back(Move(from, to));
                }
            }
            break;
        }
        // ВСЕ ЧТО ВЫШЕ ОБЪЕДИНИТЬ В 1 ФУНКЦИЮ НАДО (КОРОЛЬ, КОНЬ)
        
        case PieceType::Bishop: {
            // bishop search move mechanic
            const int BishopMoves[4][2] = {
                {1, -1}, {1, 1},
                {-1, -1}, {-1, 1},
            };
            for (int i = 0; i < 4; ++i) {
                int stepX = BishopMoves[i][0];
                int stepY = BishopMoves[i][1];
        
                // ИДЕМ В НАПРАВЛЕНИИ
                int newX = x + stepX;
                int newY = y + stepY;
        
                // ЦИКЛ ПОКА В ПОЛЕ 
                while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    int to = newY * 8 + newX;
                    Piece targetPiece = position.at(to);
                    
                    // ПУСТО МОЖНО!
                    if (targetPiece.isEmpty())
                        moves.push_back(Move(from, to));
                    
                    // ВСЕ ЧУЖАЯ ФИГУРА, КУШАТЬ И ВСТАТЬ
                    else if (targetPiece.color != myPiece.color) {
                        moves.push_back(Move(from, to));
                        break;
                    }
                    else break;
        
                    newX += stepX;
                    newY += stepY;
                }
            }
            break;
        }

        case PieceType::Rook: {
            // rook search move mechanic 
            const int RookMoves [4][2] = {
                {1, 0}, {-1, 0},
                {0, 1}, {0, -1},
            };
            for (int i = 0; i < 4; ++i) {
                int stepX = RookMoves[i][0];
                int stepY = RookMoves[i][1];
        
                // ИДЕМ В НАПРАВЛЕНИИ
                int newX = x + stepX;
                int newY = y + stepY;
                
                // ЦИКЛ ПОКА В ПОЛЕ 
                while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    int to = newY * 8 + newX;
                    Piece targetPiece = position.at(to);
                    
                    // ПУСТО МОЖНО!
                    if (targetPiece.isEmpty())
                        moves.push_back(Move(from, to));
                    
                    // ВСЕ ЧУЖАЯ ФИГУРА, КУШАТЬ И ВСТАТЬ
                    else if (targetPiece.color != myPiece.color) {
                        moves.push_back(Move(from, to));
                        break;
                    }
                    else break;
        
                    newX += stepX;
                    newY += stepY;
                }
            }
            break;
        }

        case PieceType::Queen: {
        // queen search move mechanic 
            const int QueenMoves[8][2] = {
                {1, -1}, {1, 1},
                {-1, -1}, {-1, 1},
                {1, 0}, {-1, 0},
                {0, 1}, {0, -1},
            };
            for (int i = 0; i < 8; ++i) {
                int stepX = QueenMoves[i][0];
                int stepY = QueenMoves[i][1];
        
                // ИДЕМ В НАПРАВЛЕНИИ
                int newX = x + stepX;
                int newY = y + stepY;
                
                // ЦИКЛ ПОКА В ПОЛЕ
                while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    int to = newY * 8 + newX;
                    Piece targetPiece = position.at(to);
                    
                    // ПУСТО МОЖНО!
                    if (targetPiece.isEmpty())
                        moves.push_back(Move(from, to));
                    
                    // ВСЕ ЧУЖАЯ ФИГУРА, КУШАТЬ И ВСТАТЬ
                    else if (targetPiece.color != myPiece.color) {
                        moves.push_back(Move(from, to));
                        break;
                    }
                    else break;
        
                    newX += stepX;
                    newY += stepY;
                }
            }
            break;
        }
        // ВСЕ ЧТО ВЫШЕ ОБЪЕДИНИТЬ В 1 ФУНКЦИЮ НАДО (СЛОН, ЛАДЬЯ, КОРОЛЕВА)

        case PieceType::Pawn: {
            // Направление движения пешки
            int direction = myPiece.color == Color::White ? -1 : 1;

            // Определяем начальную горизонталь
            bool isStartingRank =
                (myPiece.color == Color::White && y == 6) ||
                (myPiece.color == Color::Black && y == 1);

            // 1. Обычный ход вперёд
            int newY = y + direction;

            if (newY >= 0 && newY < 8) {
                int to = newY * 8 + x;
                Piece targetPiece = position.at(to);
            
                if (targetPiece.isEmpty()) {
                    moves.push_back(Move(from, to));
                
                    // 2. Двойной ход с начальной горизонтали
                    if (isStartingRank) {
                        int doubleY = y + direction * 2;
                        int doubleTo = doubleY * 8 + x;
                    
                        if (position.at(doubleTo).isEmpty()) {
                            moves.push_back(Move(from, doubleTo));
                        }
                    }
                }
                // 3. Диагональные взятия
                for (int dx : {-1, 1}) {
                    int captureX = x + dx;
                    int captureY = y + direction;
                
                    if (captureX >= 0 && captureX < 8) {
                        int captureTo = captureY * 8 + captureX;
                        Piece targetPiece = position.at(captureTo);
                    
                        if (!targetPiece.isEmpty() &&
                            targetPiece.color != myPiece.color) {
                            moves.push_back(Move(from, captureTo));
                        }
                    }
                }
            }
        
            break;
        }       
    }
    return moves;
}