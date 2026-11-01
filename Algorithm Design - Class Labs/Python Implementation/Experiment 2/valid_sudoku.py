def is_valid_sudoku(board):
    for i in range(9):
        row = [x for x in board[i] if x != '.']
        col = [board[r][i] for r in range(9) if board[r][i] != '.']
        if len(row) != len(set(row)) or len(col) != len(set(col)): return False
    for br in range(0,9,3):
        for bc in range(0,9,3):
            vals = [board[r][c] for r in range(br,br+3) for c in range(bc,bc+3) if board[r][c] != '.']
            if len(vals) != len(set(vals)): return False
    return True

if __name__ == "__main__":
    board = [['.']*9 for _ in range(9)]
    print(is_valid_sudoku(board))
