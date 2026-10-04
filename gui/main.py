'''main file'''

# ПЛАНЫ НА ЗАВТРА ну и ближайшее будующее: 
# кароче надо переопределить куда нить константы и тп базис хуйню, так же как будто туда 
# можно будет закинуть отрисовку спрайтов

# насчет спрайтов сделать нормальный фикс размерности, (по идее достаточно поделить размер фигуры 
# на размер клетки и поделить спрайт на получившуюся разность(скорее всего(я так думаю(наверное(я надеюсь)))))

# разделить функцию на 3 функции (как минимум чтото модно вообще написать не как функцию, а просто как подгрузку
# через импорт) + надо всегда отрисовывать перекрывающий все поле, а затем над ним перекрывающие все фигуры.start_gui

# если повезет добавить заготовку под юай оценки позиции

# вспомнить как передвигать обьекты мышкой, написать тестовый файл, для перехода фигуры с одной клетки на другую на
# четырехклеточном поле с фиксом расположения по середине клетки.

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

Board = BoardView(10, 10, CELL_SIZE, screen)
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


   
"""

    for row in range(8):
        for col in range(8):
            color = WHITE if (row + col) % 2 == 0 else BLACK
            x = x0 + col * CELL_SIZE
            y = y0 + row * CELL_SIZE
            pygame.draw.rect(screen, color, (x, y, CELL_SIZE, CELL_SIZE))
            pos += 1  
            if chess_desk[pos-1] != '': 
                sprite = pieces_sprite[chess_desk[pos-1]] 
                screen.blit(sprite, (x, y))

"""