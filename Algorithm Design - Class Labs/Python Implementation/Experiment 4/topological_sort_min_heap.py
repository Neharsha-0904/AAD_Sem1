import heapq
def topological_sort(n, edges):
    g=[[] for _ in range(n)]; indeg=[0]*n
    for u,v in edges: g[u].append(v); indeg[v]+=1
    heap=[i for i in range(n) if indeg[i]==0]; heapq.heapify(heap)
    order=[]
    while heap:
        u=heapq.heappop(heap); order.append(u)
        for v in g[u]:
            indeg[v]-=1
            if indeg[v]==0: heapq.heappush(heap,v)
    if len(order)!=n: raise ValueError("Graph contains a cycle")
    return order

if __name__ == "__main__":
    print(topological_sort(6,[(5,2),(5,0),(4,0),(4,1),(2,3),(3,1)]))
