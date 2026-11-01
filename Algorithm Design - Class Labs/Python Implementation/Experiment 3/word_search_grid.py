def exist(board, w):
    if not w: return True
    rows, cols = len(board), len(board[0]) if board else 0
    def dfs(r,c,i):
        if i == len(w): return True
        if r<0 or r>=rows or c<0 or c>=cols or board[r][c] != w[i]: return False
        ch=board[r][c]; board[r][c]="#"
        found = (dfs(r+1,c,i+1) or dfs(r-1,c,i+1) or
                 dfs(r,c+1,i+1) or dfs(r,c-1,i+1))
        board[r][c]=ch
        return found
    return any(dfs(r,c,0) for r in range(rows) for c in range(cols))

if __name__ == "__main__":
    grid=[['A','B'],['C','D']]
    print(exist(grid,"ABD"))
