class Node:
    def __init__(self, ch, next=None): self.ch, self.next = ch, next
def longest_increasing(head):
    best=[]; run=[]
    while head:
        if not run or head.ch > run[-1]: run.append(head.ch)
        else: run=[head.ch]
        if len(run)>len(best): best=run[:]
        head=head.next
    return ''.join(best)
if __name__=="__main__":
    h=Node('a',Node('b',Node('d',Node('c',Node('e')))))
    print(longest_increasing(h))
