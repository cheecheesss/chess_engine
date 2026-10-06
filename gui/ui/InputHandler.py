'''Handle mouse interaction with the chess board.'''
import pygame

class InputHandler:
	def __init__(self, board_view, board):
		self.board_view = board_view
		self.board = board
		self.selected_index = None
		self.selected_piece = None

	def handle_event(self, event):
		'''Handle mouse events for selecting and moving pieces.'''
		if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
		# if lmb:
			index = self.board_view.cell_at(event.pos)
			if index is not None and self.board[index] is not None:
			# if have piece on cell:
				self.selected_index = index
				self.selected_piece = self.board[index]

		elif event.type == pygame.MOUSEBUTTONUP and event.button == 1:
		# id releasing mouse btn:
			if self.selected_index is None:
			# if cursor dint have piece break func
				return

			destination = self.board_view.cell_at(event.pos)
			if destination is not None:
			# write piece to new cell and delete from old cell
				self.board[self.selected_index] = None
				self.board[destination] = self.selected_piece
			self.selected_index = None
			self.selected_piece = None

		
