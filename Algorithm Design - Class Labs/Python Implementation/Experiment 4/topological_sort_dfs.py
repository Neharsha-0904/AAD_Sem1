def topological_sort(n, edges):
    graph=[[] for _ in range(n)]
    for u,v in edges: graph[u].append(v)
    state=[0]*n; order=[]
    def dfs(u):
        if state[u]==1: raise ValueError("Graph contains a cycle")
        if state[u]==2: return
        state[u]=1
        for v in graph[u]: dfs(v)
        state[u]=2; order.append(u)
    for u in range(n):
        if state[u]==0: dfs(u)
    return order[::-1]

if __name__ == "__main__":
    print(topological_sort(6,[(5,2),(5,0),(4,0),(4,1),(2,3),(3,1)]))
