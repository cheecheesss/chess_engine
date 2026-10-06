import pygame


class Button(pygame.Rect):
    def __init__(self, x, y, width, height, color, text, action=None):
        super().__init__(x, y, width, height)
        self.color = color
        self.text = text
        self.action = action

    def draw(self, screen, font, text_color):
        pygame.draw.rect(screen, self.color, self)

        label = font.render(self.text, True, text_color)
        label_rect = label.get_rect(center=self.center)
        screen.blit(label, label_rect)

    def handle_event(self, event):
        if (
            event.type == pygame.MOUSEBUTTONDOWN
            and event.button == 1
            and self.collidepoint(event.pos)
        ):
            if self.action:
                self.action()
        