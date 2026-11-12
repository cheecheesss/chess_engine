'''main file'''

import pygame 

from ui.config import *
from ui.BoardView import BoardView
from ui.Board import chess_board, reset_board
from ui.InputHandler import InputHandler
from ui.sprites import load_pieces
from ui.Buttons import Button
from ui.PosScale import PosScale

from engine_client import EngineClient

def main():
    '''main func for create window 
    and update him'''
    pygame.init()
    engine = EngineClient()

    try:
        responce = engine.start()
        chess_board[:] = responce.board
        state = {"status": responce.status}

        def send_move(from_square, to_square):
            try:
                responce = engine.play_move(from_square, to_square)
            except (ValueError, RuntimeError) as error:
                print(f"Ход не принят: {error}")
                return
            
            chess_board[:] = responce.board
            state["status"] = responce.status

        # create screen 
        screen = pygame.display.set_mode(DISPLAY_SIZE)
        pygame.display.set_caption("Chess engine")

        # create board view, input handler and load pieces,
        # pos scale and start position button
        board_view = BoardView(30, 30, CELL_SIZE, screen)
        pieces = load_pieces(CELL_SIZE)

        input_handler = InputHandler(
            board_view,
            chess_board,
            on_move = send_move,
        )

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

            # GUI drawing
            screen.fill(GRAY)
            board_view.draw_board()
            board_view.draw_pieces(chess_board, pieces, input_handler.selected_index)
            board_view.draw_dragged_piece(
                input_handler.selected_piece,
                pieces,
                pygame.mouse.get_pos(),
            )

            pygame.display.flip() # GUI update
            clock.tick(FPS) # fps limiter 
    finally:
        engine.close()
        pygame.quit()


if __name__ == '__main__':  
    main()
