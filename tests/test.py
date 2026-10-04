'''разбирательства'''

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
    '''load all piece sprites'''
    return {
        key: pygame.transform.smoothscale(
            pygame.image.load(filename).convert_alpha(), # huinya
            (cell_size, cell_size),
        )
        for key, filename in pieces_sprite.items()
    }   

p = {1: 1, 2: 2, 3: 4}
print(p.items()) 

# .items() переводит словарь в вид [(key, val), (key, val)]
for key, val in p.items():
    print(key , val)