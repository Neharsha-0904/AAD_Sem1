from math import hypot
def cross(o,a,b): return (a[0]-o[0])*(b[1]-o[1])-(a[1]-o[1])*(b[0]-o[0])
def convex_hull(p):
    p=sorted(set(map(tuple,p)))
    if len(p)<=1:return p
    lo=[]
    for x in p:
        while len(lo)>=2 and cross(lo[-2],lo[-1],x)<=0:lo.pop()
        lo.append(x)
    up=[]
    for x in reversed(p):
        while len(up)>=2 and cross(up[-2],up[-1],x)<=0:up.pop()
        up.append(x)
    return lo[:-1]+up[:-1]
def hull_perimeter(h):
    if len(h)<2:return 0.0
    return sum(hypot(h[(i+1)%len(h)][0]-h[i][0],h[(i+1)%len(h)][1]-h[i][1]) for i in range(len(h)))
if __name__=="__main__":
    h=convex_hull([(0,0),(0,1),(1,0),(1,1)])
    print("Hull:",h);print("Perimeter:",hull_perimeter(h))
