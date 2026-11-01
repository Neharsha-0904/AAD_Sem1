def minimum_hamiltonian_cycle(graph, start=0):
    n=len(graph); best_cost=float("inf"); best_path=[]
    used={start}; path=[start]
    def dfs(u,cost):
        nonlocal best_cost,best_path
        if len(path)==n:
            if graph[u][start] is not None and cost+graph[u][start]<best_cost:
                best_cost=cost+graph[u][start]; best_path=path[:] + [start]
            return
        for v in range(n):
            if v not in used and graph[u][v] is not None and cost+graph[u][v]<best_cost:
                used.add(v); path.append(v)
                dfs(v,cost+graph[u][v])
                path.pop(); used.remove(v)
    dfs(start,0)
    return (best_cost,best_path) if best_path else (None,[])
if __name__=="__main__":
    g=[[None,10,15,20],[10,None,35,25],[15,35,None,30],[20,25,30,None]]
    print(minimum_hamiltonian_cycle(g))
