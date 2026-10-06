'''Handle mouse interaction with the chess board.'''

class InputHandler:
	def __init__(self, board_view, board):
		self.board_view = board_view
		self.board = board
		self.selected_index = None
		self.selected_piece = None

	def handle_event(self, event):
		import pygame

		if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
			index = self.board_view.cell_at(event.pos)
			if index is not None and self.board[index] is not None:
				self.selected_index = index
				self.selected_piece = self.board[index]

		elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
			if self.selected_index is None:
				return

			destination = self.board_view.cell_at(event.pos)
			if destination is not None:
				self.board[self.selected_index] = None
				self.board[destination] = self.selected_piece

			self.selected_index = None
			self.selected_piece = None

		
