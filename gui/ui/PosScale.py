'''A vertical scale to visualize the evaluation score of the chess engine.'''

import pygame
from ui.config import WHITE, BLACK, GRAY, font

class PosScale(pygame.Rect):
    def __init__(self, x, y, width, height):
        super().__init__(x, y, width, height)

    def black_height(self, pos):
        """Return the black area height for a score in the [-10, 10] range."""
        pos = max(-10, min(10, pos)) # clamp pos to [-10, 10]
        return round((10 - pos) * self.height / 20) 

    def draw(self, screen, pos):
        '''Draw the scale on the screen.'''
        pygame.draw.rect(screen, WHITE, self)

        black_height = self.black_height(pos)
        pygame.draw.rect(
            screen,
            BLACK,
            (self.x, self.y, self.width, black_height),
        )

        text = font.render(f'{pos}', True, GRAY)
        screen.blit(
            text,
            (
                self.x + self.width // 2 - text.get_width() // 2,
                self.y + self.height // 2 - text.get_height() // 2,
            ),
        )
