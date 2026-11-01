class Node:
    def __init__(self, coeff, exp):
        self.coeff, self.exp, self.next = coeff, exp, None

def build(terms):
    head = tail = None
    for c, e in sorted(terms, key=lambda t: -t[1]):
        node = Node(c, e)
        if tail: tail.next = node
        else: head = node
        tail = node
    return head

def add_poly(a, b):
    dummy = tail = Node(0, 0)
    while a or b:
        if b is None or (a and a.exp > b.exp):
            c, e = a.coeff, a.exp; a = a.next
        elif a is None or b.exp > a.exp:
            c, e = b.coeff, b.exp; b = b.next
        else:
            c, e = a.coeff + b.coeff, a.exp
            a, b = a.next, b.next
        if c:
            tail.next = Node(c, e); tail = tail.next
    return dummy.next

def display(head):
    terms = []
    while head:
        terms.append(f"{head.coeff}x^{head.exp}")
        head = head.next
    print(" + ".join(terms) if terms else "0")

if __name__ == "__main__":
    p = build([(3,2),(2,1),(5,0)])
    q = build([(4,3),(3,1),(2,0)])
    display(add_poly(p,q))
