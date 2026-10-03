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
from pygame import transform, image
from ui.colors import GRAY, WHITE, BLACK

pygame.init()


# константы и переменные которые надо куда то опеределить 
# (например в файл конфиг которого у меня нет потому что я чмооооо)

# create screen 
screen = pygame.display.set_mode((600, 400))
screen.fill(GRAY)
pygame.display.set_caption("Chess engine")

# for fps limit
clock = pygame.time.Clock() 
FPS = 30

def start_gui():
    while True: 
    # infinity loop
        for event in pygame.event.get():
        # event handler
            if event.type == pygame.QUIT:
                # closing with a cross
                pygame.quit() 

        pygame.display.flip() # GUI update
        clock.tick(FPS) # fps limiter 

if __name__ == '__main__':
    start_gui()

   
"""
class board: 
    screen, color, (x, y, width, height)
    def __init__(self): 
    так же нужно отрисовать фигуры, текст и тд в ините, но пока только с доской работаю

def create_board(screen: pygame.surface.Surface, 
                CELL_SIZE: int = 40, 
                x0: int = 40, y0: int = 40):
    '''create chess board (dont have letter and pieces)''' 
    pos = 0

    chess_desk = [
        'b_r', 'b_n', 'b_b', 'b_q', 'b_k', 'b_b', 'b_n', 'b_r',
        'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p',
        '',    '',    '',    '',    '',    '',    '',    '',
        '',    '',    '',    '',    '',    '',    '',    '',
        '',    '',    '',    '',    '',    '',    '',    '',
        '',    '',    '',    '',    '',    '',    '',    '',
        'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p',
        'w_r', 'w_n', 'w_b', 'w_q', 'w_k', 'w_b', 'w_n', 'w_r',
    ]

    # 'b_b': transform.scale(image.load('gui/assets/pieces/b_bishop.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
    pieces_sprite = {
        'b_b': transform.scale(image.load('gui/assets/pieces/b_bishop.png').convert_alpha(),(CELL_SIZE, CELL_SIZE)),
        'b_k': transform.scale(image.load('gui/assets/pieces/b_king.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'b_n': transform.scale(image.load('gui/assets/pieces/b_knight.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'b_p': transform.scale(image.load('gui/assets/pieces/b_pawn.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'b_q': transform.scale(image.load('gui/assets/pieces/b_queen.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'b_r': transform.scale(image.load('gui/assets/pieces/b_rook.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),       
        'w_b': transform.scale(image.load('gui/assets/pieces/w_bishop.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'w_k': transform.scale(image.load('gui/assets/pieces/w_king.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'w_n': transform.scale(image.load('gui/assets/pieces/w_knight.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'w_p': transform.scale(image.load('gui/assets/pieces/w_pawn.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'w_q': transform.scale(image.load('gui/assets/pieces/w_queen.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
        'w_r': transform.scale(image.load('gui/assets/pieces/w_rook.png').convert_alpha(), (CELL_SIZE, CELL_SIZE)),
    }
    
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

create_board(screen, 40, 10, 10)
"""