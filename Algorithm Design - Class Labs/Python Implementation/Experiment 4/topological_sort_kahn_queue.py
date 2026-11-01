from collections import deque
def topological_sort(n, edges):
    g=[[] for _ in range(n)]; indeg=[0]*n
    for u,v in edges: g[u].append(v); indeg[v]+=1
    q=deque(i for i in range(n) if indeg[i]==0); order=[]
    while q:
        u=q.popleft(); order.append(u)
        for v in g[u]:
            indeg[v]-=1
            if indeg[v]==0: q.append(v)
    if len(order)!=n: raise ValueError("Graph contains a cycle")
    return order

if __name__ == "__main__":
    print(topological_sort(6,[(5,2),(5,0),(4,0),(4,1),(2,3),(3,1)]))
