'''class BoardView for GUI'''

import pygame
from ui.config import BLACK, BLUE, WHITE, font


class BoardView: 
    '''draw board for screen(window)'''
    def __init__(self, x0, y0, cell_size, screen):
        self.x0 = x0
        self.y0 = y0
        self.cell_size = cell_size
        self.screen = screen

    def draw_board(self, board_font=None): 
        for row in range(8):
            for col in range(8): 
                color = WHITE if (row + col) % 2 == 0 else BLUE
                
                x = self.x0 + col * self.cell_size
                y = self.y0 + row * self.cell_size

                pygame.draw.rect(
                    self.screen, color, 
                    (x, y, self.cell_size, self.cell_size)
                )

        if board_font is None:
            board_font = font

        letters = 'abcdefgh'
        for col, letter in enumerate(letters):
            label = board_font.render(letter, True, BLACK)
            x = self.x0 + col * self.cell_size + (self.cell_size - label.get_width()) // 2
            y = self.y0 + 8 * self.cell_size + 8
            self.screen.blit(label, (x, y))

        for row, number in enumerate(range(8, 0, -1)):
            label = board_font.render(str(number), True, BLACK)
            x = self.x0 - label.get_width() - 8
            y = self.y0 + row * self.cell_size + (self.cell_size - label.get_height()) // 2
            self.screen.blit(label, (x, y))

    def cell_at(self, position):
        '''Return the index of the cell at the given position, 
        or None if outside the board. (cursor position on board)'''
        x, y = position
        col = (x - self.x0) // self.cell_size
        row = (y - self.y0) // self.cell_size

        if 0 <= row < 8 and 0 <= col < 8:
            # if on the board:
            return row * 8 + col
        # else:
        return None

    def draw_pieces(self, board, pieces, hidden_index=None):
        for index, piece in enumerate(board):
            if piece is None or index == hidden_index:
                continue

            row, col = divmod(index, 8)

            # top left corner
            cell_x = self.x0 + col * self.cell_size
            cell_y = self.y0 + row * self.cell_size

            sprite = pieces[piece]

            # centering the sprite relative to the cell
            x = cell_x + (self.cell_size - sprite.get_width()) // 2
            y = cell_y + (self.cell_size - sprite.get_height()) // 2

            self.screen.blit(sprite, (x, y))
            

    def draw_dragged_piece(self, piece, pieces, position):
        if piece is None:
            # break func 
            return

        sprite = pieces[piece]
        x = position[0] - sprite.get_width() // 2
        y = position[1] - sprite.get_height() // 2
        self.screen.blit(sprite, (x, y))