def knapsack(ws, vs, cap):
    dp = [0]*(cap+1)
    for w,v in zip(ws,vs):
        for c in range(cap,w-1,-1):
            dp[c] = max(dp[c], v + dp[c-w])
    return dp[cap]

if __name__ == "__main__":
    print(knapsack([1,3,4],[15,20,30],4))
