def cross(o,a,b): return (a[0]-o[0])*(b[1]-o[1])-(a[1]-o[1])*(b[0]-o[0])
def convex_hull(p):
    p=sorted(set(map(tuple,p)))
    if len(p)<=1: return p
    lo=[]
    for x in p:
        while len(lo)>=2 and cross(lo[-2],lo[-1],x)<=0: lo.pop()
        lo.append(x)
    up=[]
    for x in reversed(p):
        while len(up)>=2 and cross(up[-2],up[-1],x)<=0: up.pop()
        up.append(x)
    return lo[:-1]+up[:-1]
if __name__=="__main__": print(convex_hull([(0,0),(0,1),(1,0),(1,1)]))
