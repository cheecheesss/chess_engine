'''file for load all sprites'''

import pygame

from ui.config import CELL_SIZE


pieces_sprite = {
    'b_b': 'gui/assets/pieces/b_bishop.png',
    'b_k': 'gui/assets/pieces/b_king.png',
    'b_n': 'gui/assets/pieces/b_knight.png',
    'b_p': 'gui/assets/pieces/b_pawn.png',
    'b_q': 'gui/assets/pieces/b_queen.png',
    'b_r': 'gui/assets/pieces/b_rook.png',

    'w_b': 'gui/assets/pieces/w_bishop.png',
    'w_k': 'gui/assets/pieces/w_king.png',
    'w_n': 'gui/assets/pieces/w_knight.png',
    'w_p': 'gui/assets/pieces/w_pawn.png',
    'w_q': 'gui/assets/pieces/w_queen.png',
    'w_r': 'gui/assets/pieces/w_rook.png',
}


def load_pieces(cell_size: int = CELL_SIZE) -> dict[str, pygame.Surface]:
    '''Load all piece sprites and fit them inside the cell without distortion.'''

    pieces = {}

    for key, filename in pieces_sprite.items():
        image = pygame.image.load(filename).convert_alpha()

        # Исходный размер спрайта
        width, height = image.get_size()

        # Масштабируем пропорционально,
        # чтобы фигура целиком помещалась в клетку
        scale = min(
            cell_size / width,
            cell_size / height,
        )

        new_width = round(width * scale) - (cell_size // 20)
        new_height = round(height * scale) - (cell_size // 20)

        pieces[key] = pygame.transform.smoothscale(
            image,
            (new_width, new_height),
        )

    return pieces