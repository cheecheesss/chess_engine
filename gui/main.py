'''main file'''

# если повезет добавить заготовку под юай оценки позиции


# вспомнить как передвигать обьекты мышкой, написать тестовый файл, для перехода фигуры с одной клетки на другую на
# четырехклеточном поле с фиксом расположения по середине клетки.

# (надо соотносить визуал с отрисовкой, я так понял если я сразу сделаю чтобы отцентрализовывалось
# то все заработает как надо, но надо как то соотносить файлы boardview и board (чтобы при 
# перемещении фигуры оно записывалось в список (None = w_p например))

# затем перенести механику на более большой масштаб нормального поля

# добавить обозначения клеткам  

# закончить и оптимизировать все эти планы = закончить гуи и начать учить спп, даю себе примерно 4 дня

import pygame 
from ui.config import DISPLAY_SIZE, CELL_SIZE, clock, FPS, GRAY
from ui.BoardView import BoardView
from ui.Board import chess_board
from ui.sprites import load_pieces

pygame.init()

# create screen 
screen = pygame.display.set_mode(DISPLAY_SIZE)
screen.fill(GRAY)
pygame.display.set_caption("Chess engine")

Board = BoardView(30, 30, CELL_SIZE, screen)
pieces = load_pieces(CELL_SIZE)


def main():
    '''main func for create window 
    and update him'''
    start = True
    while start: 
    # infinity loop
        for event in pygame.event.get():
        # event handler
            if event.type == pygame.QUIT:
                # closing with a cross 
                start = False
                break



        # GUI logic
        Board.draw_board()
        Board.draw_pieces(chess_board, pieces)



        pygame.display.flip() # GUI update
        clock.tick(FPS) # fps limiter 

    pygame.quit()


if __name__ == '__main__':  
    main()
