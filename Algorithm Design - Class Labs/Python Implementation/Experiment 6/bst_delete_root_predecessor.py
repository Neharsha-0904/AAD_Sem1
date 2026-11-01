class Node:
    def __init__(self, key, l=None, r=None):
        self.key,self.left,self.right=key,l,r
def delete_max(node):
    if node.right is None: return node.left, node.key
    node.right, v = delete_max(node.right)
    return node, v
def delete_root_with_predecessor(root):
    if root is None: return None
    if root.left is None: return root.right
    if root.right is None: return root.left
    root.left, pred = delete_max(root.left)
    root.key = pred
    return root
def inorder(node):
    return inorder(node.left)+[node.key]+inorder(node.right) if node else []
if __name__=="__main__":
    root=Node(50,Node(30,Node(20),Node(40)),Node(70))
    root=delete_root_with_predecessor(root)
    print(inorder(root))
