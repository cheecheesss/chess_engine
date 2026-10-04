'''class BoardView for GUI'''

import pygame
from ui.config import BLACK, WHITE


class BoardView: 
    '''draw board for screen(windoow)'''
    def __init__(self, x0, y0, cell_size, screen):
        self.x0 = x0
        self.y0 = y0
        self.cell_size = cell_size
        self.screen = screen


    def draw_board(self): 
        for row in range(8):
            for col in range(8): 
                color = WHITE if (row + col) % 2 == 0 else BLACK
                
                x = self.x0 + col * self.cell_size
                y = self.y0 + row * self.cell_size

                pygame.draw.rect(
                    self.screen, color, 
                    (x, y, self.cell_size, self.cell_size)
                )

    def draw_pieces(self, board, pieces): 
        for index, piece in enumerate(board):
            if piece == None:
                continue
            row, col = divmod(index, 8)
            x = self.x0 + col * self.cell_size
            y = self.y0 + row * self.cell_size
            self.screen.blit(pieces[piece], (x, y))