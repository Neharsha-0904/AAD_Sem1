def min_cuts(s):
    n=len(s)
    pal=[[False]*n for _ in range(n)]
    cuts=[0]*n
    for end in range(n):
        best=end
        for start in range(end+1):
            if s[start]==s[end] and (end-start<2 or pal[start+1][end-1]):
                pal[start][end]=True
                best=0 if start==0 else min(best,cuts[start-1]+1)
        cuts[end]=best
    return cuts[-1] if n else 0
if __name__=="__main__": print(min_cuts("aab"))
