'''class BoardView for GUI'''

import pygame
from ui.config import BLUE, WHITE


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
                color = WHITE if (row + col) % 2 == 0 else BLUE
                
                x = self.x0 + col * self.cell_size
                y = self.y0 + row * self.cell_size

                pygame.draw.rect(
                    self.screen, color, 
                    (x, y, self.cell_size, self.cell_size)
                )

    def draw_pieces(self, board, pieces):
        for index, piece in enumerate(board):
            if piece is None:
                continue

            row, col = divmod(index, 8)

            # top left corner
            cell_x = self.x0 + col * self.cell_size
            cell_y = self.y0 + row * self.cell_size

            sprite = pieces[piece]

            # сentering the sprite relative to the cell
            x = cell_x + (self.cell_size - sprite.get_width()) // 2
            y = cell_y + (self.cell_size - sprite.get_height()) // 2

            self.screen.blit(sprite, (x, y))