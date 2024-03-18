"""
sources:
    https://en.wikipedia.org/wiki/Minimax
    https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
"""

import numpy as np
import pygame
import sys
import random

YELLOW = (30, 255, 0)
DARK_YELLOW = (15, 130, 0)
RED = (255, 0, 0)
DARK_RED = (194, 0, 0)

BACKGROUND = (33, 41, 38)
FOREGROUND = (148, 209, 188)

ROW_COUNT = 6
COLUMN_COUNT = 7

PLAYER = 0
AI = 1

EMPTY = 0
PLAYER_PIECE = 1
AI_PIECE = 2

def create_board():
    board = np.zeros((6, 7)).astype('uint8')
    return board

def drop_piece(board, row, col, piece):
    board[row][col] = piece

def is_valid_location(board, col):
    return board[5][col] == 0

def get_next_open_row(board, col):
    for r in range(ROW_COUNT):
        if board[r][col] == 0:
            return r

def print_board(board):
    print(f"  {'   '.join(list(map(str, range(7))))}")
    for row in reversed(board):
        print(f"| {' | '.join(list(map(str, row)))} |")
    print()

def winning_move(board, piece):
    # horizontal
    for x in range(ROW_COUNT):
        for y in range(COLUMN_COUNT-3):
            if all(board[x, y:y+4] == piece):
                return True

    # vertical
    for x in range(ROW_COUNT-3):
        for y in range(COLUMN_COUNT):
            if all(board[x:x+4, y] == piece):
                return True

    # diagonal
    for x in range(ROW_COUNT-3):
        for y in range(COLUMN_COUNT-3):
            if all([board[x+i, y+i] == piece for i in range(4)]) or\
               all([board[x+3-i, y+i] == piece for i in range(4)]):
                return True

    return False

def window_evaluate(window, piece):
    score = 0

    opp_piece = PLAYER_PIECE
    if piece == opp_piece:
        opp_piece = AI_PIECE

    if (window == piece).sum() == 4:
        score += 100
    elif (window == piece).sum() == 3 and (window == EMPTY).sum() == 1:
        score += 5
    elif  (window == piece).sum() == 2 and  (window == EMPTY).sum() == 2:
        score += 2

    if (window == opp_piece).sum() == 3 and (window == EMPTY).sum() == 1:
        score -= 4
    
    return score

def score_position(board, piece):
    score = 0

    # center column
    center_array = board[:, COLUMN_COUNT//2]
    center_count = (center_array == piece).sum()
    score += center_count * 3

    # horizontal
    for r in range(ROW_COUNT):
        row = board[r, :]
        for c in range(COLUMN_COUNT-3):
            window = row[c:c+4]
            score += window_evaluate(window, piece)

    # vertical
    for c in range(COLUMN_COUNT):
        col = board[:, c]
        for r in range(ROW_COUNT-3):
            window = col[r:r+4]
            score += window_evaluate(window, piece)
    
    # diagonal
    for r in range(ROW_COUNT-3):
        for c in range(COLUMN_COUNT-3):
            diagonal_left_window = np.array([board[r+i, c+i] for i in range(4)])
            diagonal_right_window = np.array([board[r+i, c+3-i] for i in range(4)])
            score += window_evaluate(diagonal_left_window, piece)
            score += window_evaluate(diagonal_right_window, piece)
    
    return score

def is_terminal_node(board):
    return winning_move(board, PLAYER_PIECE) or\
           winning_move(board, AI_PIECE) or\
           len(get_valid_locations(board)) == 0

def minimax(board, depth, alpha, beta, maximizingPlayer):
    valid_locations = get_valid_locations(board)
    is_terminal = is_terminal_node(board)

    if depth == 0 or is_terminal:
        if is_terminal:
            if winning_move(board, AI_PIECE):
                return (None, 100000)
            elif winning_move(board, PLAYER_PIECE):
                return (None, -100000)
            else: # Game is over (no more valid moves)
                return(None, 0)
        else: # Depth is zero
            return (None, score_position(board, AI_PIECE))
        
    if maximizingPlayer:
        value = -float('inf')
        column = random.choice(valid_locations)
        for col in valid_locations:
            row = get_next_open_row(board, col)
            board_copy = board.copy()
            drop_piece(board_copy, row, col, AI_PIECE)
            new_score = minimax(board_copy, depth-1, alpha, beta, False)[1]

            if new_score > value:
                value = new_score
                column = col

            alpha = max(alpha, value)
            if alpha >= beta:
                break

        return column, value
    
    else:
        value = float('inf')
        column = random.choice(valid_locations)
        for col in valid_locations:
            row = get_next_open_row(board, col)
            board_copy = board.copy()
            drop_piece(board_copy, row, col, PLAYER_PIECE)
            new_score = minimax(board_copy, depth-1, alpha, beta, True)[1]

            if new_score < value:
                value = new_score
                column = col

            beta = min(beta, value)
            if alpha >= beta:
                break

        return column, value

def get_valid_locations(board):
    valid_locations = []
    for col in range(COLUMN_COUNT):
        if is_valid_location(board, col):
            valid_locations.append(col)
    return valid_locations

def pick_best_move(board, piece):
    valid_locations = get_valid_locations(board)

    best_score = -10000
    best_col = random.choice(valid_locations)

    for col in valid_locations:
        row = get_next_open_row(board, col)
        temp_board = board.copy()
        drop_piece(temp_board, row, col, piece)
        score = score_position(temp_board, piece)
        if score > best_score:
            best_score = score
            best_col = col

    return best_col

def draw_board(board):
    for c in range(COLUMN_COUNT):
        for r in range(ROW_COUNT):
            pygame.draw.rect(screen, FOREGROUND, (c*SQUARESIZE, (ROW_COUNT-r)*SQUARESIZE, SQUARESIZE, SQUARESIZE))
            if board[r][c] == 0:
                pygame.draw.circle(screen, BACKGROUND, ((c+0.5)*SQUARESIZE, (ROW_COUNT-r+0.5)*SQUARESIZE), 0.4*SQUARESIZE)
            if board[r][c] == PLAYER_PIECE:
                pygame.draw.circle(screen, DARK_RED, ((c+0.5)*SQUARESIZE, (ROW_COUNT-r+0.5)*SQUARESIZE), 0.43*SQUARESIZE)
                pygame.draw.circle(screen, RED, ((c+0.5)*SQUARESIZE, (ROW_COUNT-r+0.5)*SQUARESIZE), 0.4*SQUARESIZE)
            if board[r][c] == AI_PIECE  :
                pygame.draw.circle(screen, DARK_YELLOW, ((c+0.5)*SQUARESIZE, (ROW_COUNT-r+0.5)*SQUARESIZE), 0.43*SQUARESIZE)
                pygame.draw.circle(screen, YELLOW, ((c+0.5)*SQUARESIZE, (ROW_COUNT-r+0.5)*SQUARESIZE), 0.4*SQUARESIZE)
    pygame.display.update()

board = create_board()
game_over = False
turn = random.randint(0, 1)

pygame.init()
SQUARESIZE = 100
width = COLUMN_COUNT*SQUARESIZE
height = (ROW_COUNT+1)*SQUARESIZE
size = (width, height)
myfont = pygame.font.SysFont("monospace", 75)

screen = pygame.display.set_mode(size)
draw_board(board)
pygame.draw.rect(screen, BACKGROUND, (0, 0, width, SQUARESIZE))
pygame.display.update()

while not game_over:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()

        if event.type == pygame.MOUSEMOTION:
            pygame.draw.rect(screen, BACKGROUND, (0, 0, width, SQUARESIZE))
            posx = event.pos[0]
            posx = max(0.5*SQUARESIZE, min(posx, width-0.5*SQUARESIZE))
            if turn == PLAYER:
                pygame.draw.circle(screen, DARK_RED, (posx, 0.5*SQUARESIZE), 0.43*SQUARESIZE)
                pygame.draw.circle(screen, RED, (posx, 0.5*SQUARESIZE), 0.4*SQUARESIZE)
            pygame.display.update()

        if event.type == pygame.MOUSEBUTTONDOWN:
            if turn == PLAYER:
                posx = event.pos[0]
                col = posx // SQUARESIZE

                if is_valid_location(board, col):
                    row = get_next_open_row(board, col)
                    drop_piece(board, row, col, PLAYER_PIECE)
                    draw_board(board)

                    if winning_move(board, 1):
                        pygame.draw.rect(screen, BACKGROUND, (0, 0, width, SQUARESIZE))
                        label = myfont.render("Player 1 wins", 1, RED)
                        screen.blit(label, (40, 10))
                        pygame.display.update()
                        game_over = True

                    turn = 1 if turn == 0 else 0

                    print_board(board)
                    draw_board(board)

    if turn == AI and not game_over:
        # col = random.choice(get_valid_locations())
        # col = pick_best_move(board, AI_PIECE)
        col, minimax_score = minimax(board, 4, -float('inf'), float('inf'), True)

        if is_valid_location(board, col):
            # pygame.time.wait(500)
            row = get_next_open_row(board, col)
            drop_piece(board, row, col, AI_PIECE)
            draw_board(board)

            if winning_move(board, 2):
                pygame.draw.rect(screen, BACKGROUND, (0, 0, width, SQUARESIZE))
                label = myfont.render("Player 2 wins", 1, YELLOW)
                screen.blit(label, (40, 10))
                pygame.display.update()
                game_over = True

            print_board(board)
            draw_board(board)

            turn = 1 if turn == 0 else 0

    if game_over:
        pygame.time.wait(3000)
