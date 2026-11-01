def solve_sudoku(board):
    for r in range(9):
        for c in range(9):
            if board[r][c] == 0:
                for n in range(1,10):
                    if (n not in board[r] and
                        all(board[i][c] != n for i in range(9)) and
                        all(board[i][j] != n for i in range(r//3*3,r//3*3+3)
                            for j in range(c//3*3,c//3*3+3))):
                        board[r][c] = n
                        if solve_sudoku(board): return True
                        board[r][c] = 0
                return False
    return True

if __name__ == "__main__":
    board = [[0]*9 for _ in range(9)]
    if solve_sudoku(board):
        for row in board: print(*row)
    else: print("No solution")
