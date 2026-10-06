'''board logic'''

chess_board = [
    'b_r', 'b_n', 'b_b', 'b_q', 'b_k', 'b_b', 'b_n', 'b_r',
    'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p',
    None,  None,  None,  None,  None,  None,  None,  None,
    None,  None,  None,  None,  None,  None,  None,  None,
    None,  None,  None,  None,  None,  None,  None,  None,
    None,  None,  None,  None,  None,  None,  None,  None,
    'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p',
    'w_r', 'w_n', 'w_b', 'w_q', 'w_k', 'w_b', 'w_n', 'w_r',
]

start_pos = chess_board

start_pos = chess_board.copy()

def reset_board():
    chess_board[:] = start_pos.copy()