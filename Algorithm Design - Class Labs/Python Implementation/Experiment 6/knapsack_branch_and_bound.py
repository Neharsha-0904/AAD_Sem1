import heapq
def knapsack_bb(ws, vs, cap):
    items=sorted(zip(ws,vs),key=lambda x:x[1]/x[0],reverse=True)
    n=len(items)
    def bound(level, w, v):
        if w>=cap: return v
        b=v; total=w; i=level
        while i<n and total+items[i][0]<=cap:
            total+=items[i][0]; b+=items[i][1]; i+=1
        if i<n: b+=(cap-total)*items[i][1]/items[i][0]
        return b
    best=0; heap=[(-bound(0,0,0),0,0,0)]
    while heap:
        negb,level,w,v=heapq.heappop(heap)
        if -negb<=best or level==n: continue
        wi,vi=items[level]
        if w+wi<=cap:
            nv=v+vi; best=max(best,nv)
            if level+1<n and bound(level+1,w+wi,nv)>best:
                heapq.heappush(heap,(-bound(level+1,w+wi,nv),level+1,w+wi,nv))
        if level+1<n and bound(level+1,w,v)>best:
            heapq.heappush(heap,(-bound(level+1,w,v),level+1,w,v))
    return best
if __name__=="__main__": print(knapsack_bb([2,3,5],[40,50,100],5))
