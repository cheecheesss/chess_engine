'''main file'''

# если повезет добавить заготовку под юай оценки позиции

# добавить кнопку для отмены хода, добавить кнопку для возвращения к начальной позиции
# разобраться в инпут хендлере и борд вью новых строках 

# добавить обозначения клеткам  

# закончить и оптимизировать все эти планы = закончить гуи и начать учить спп, даю себе примерно 4 дня

import pygame 
from ui.config import DISPLAY_SIZE, CELL_SIZE, clock, FPS, GRAY, WHITE, BLACK, font
from ui.BoardView import BoardView
from ui.Board import chess_board, reset_board
from ui.InputHandler import InputHandler
from ui.sprites import load_pieces
from ui.Buttons import Button
from ui.PosScale import PosScale

pygame.init()

# create screen 
screen = pygame.display.set_mode(DISPLAY_SIZE)
pygame.display.set_caption("Chess engine")

# create board view, input handler and load pieces
board_view = BoardView(30, 30, CELL_SIZE, screen)
input_handler = InputHandler(board_view, chess_board)
pieces = load_pieces(CELL_SIZE)
start_pos_btn = Button(700, 30, 100, 50, WHITE, "Start Pos", reset_board)
pos_scale = PosScale(700, 100, 70, 400)



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
            input_handler.handle_event(event)
            start_pos_btn.handle_event(event)


        # GUI drawing
        screen.fill(GRAY)
        board_view.draw_board()
        board_view.draw_pieces(chess_board, pieces, input_handler.selected_index)
        board_view.draw_dragged_piece(
            input_handler.selected_piece,
            pieces,
            pygame.mouse.get_pos(),
        )
        start_pos_btn.draw(screen, font, BLACK)
        pos_scale.draw(screen, 0.0)

        pygame.display.flip() # GUI update
        clock.tick(FPS) # fps limiter 

    pygame.quit()


if __name__ == '__main__':  
    main()
