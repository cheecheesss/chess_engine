import pygame

#  ======= GUICONFIG =======
DISPLAY_SIZE = (900, 600)
clock = pygame.time.Clock()
FPS = 30
CELL_SIZE = 60

# ======= COLORS ======= =======
WHITE = (255, 255 , 255)
BLUE = (65, 105, 225) 
BLACK = (0, 0, 0)
GRAY = (127, 127, 127) 

# ======= FONTS =======
pygame.font.init()
font = pygame.font.SysFont("arial", 30)