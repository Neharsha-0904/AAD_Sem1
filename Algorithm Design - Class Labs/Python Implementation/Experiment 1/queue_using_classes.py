from collections import deque
class Queue:
    def __init__(self): self.items = deque()
    def enqueue(self, v): self.items.append(v)
    def dequeue(self):
        if not self.items: raise IndexError("Queue is empty")
        return self.items.popleft()
    def peek(self):
        if not self.items: raise IndexError("Queue is empty")
        return self.items[0]
    def display(self): print(list(self.items))

if __name__ == "__main__":
    q = Queue()
    q.enqueue(10); q.enqueue(20); q.enqueue(30)
    q.display()
    print("Removed:", q.dequeue())
    print("Front:", q.peek())
