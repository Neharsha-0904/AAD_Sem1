class Node:
    def __init__(self, v, next=None): self.value, self.next = v, next
def delete_kth(head, k):
    if k < 1: raise ValueError("k is 1-based")
    if head is None: return None
    if k == 1: return head.next
    cur=head
    for _ in range(k-2):
        if cur.next is None: return head
        cur=cur.next
    if cur.next: cur.next=cur.next.next
    return head
def display(head):
    out=[]
    while head: out.append(str(head.value)); head=head.next
    print(" -> ".join(out))
if __name__=="__main__":
    h=Node(10,Node(20,Node(30,Node(40,Node(50,Node(60))))))
    h=delete_kth(h,5); display(h)
