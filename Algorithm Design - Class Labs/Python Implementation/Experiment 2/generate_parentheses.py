def generate_parentheses(n):
    ans = []
    def backtrack(s, op, cl):
        if len(s) == 2*n:
            ans.append(s); return
        if op < n: backtrack(s+"(", op+1, cl)
        if cl < op: backtrack(s+")", op, cl+1)
    backtrack("", 0, 0)
    return ans

if __name__ == "__main__":
    print(generate_parentheses(3))
