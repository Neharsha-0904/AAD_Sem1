from collections import deque
class Node:
    def __init__(self, v, l=None, r=None):
        self.value,self.left,self.right=v,l,r
def print_level(root, x):
    if not root or x<1: return []
    q=deque([(root,1)])
    while q:
        node,level=q.popleft()
        if level==x: return [node.value] + [v for n,l in q if l==x for v in [n.value]]
        if level<x:
            if node.left: q.append((node.left,level+1))
            if node.right: q.append((node.right,level+1))
    return []
if __name__=="__main__":
    root=Node(1,Node(2),Node(3))
    print(print_level(root,2))
