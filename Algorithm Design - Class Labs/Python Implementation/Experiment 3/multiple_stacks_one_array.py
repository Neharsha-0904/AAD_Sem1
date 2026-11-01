class KStacks:
    def __init__(self, k, cap):
        self.k, self.capacity = k, cap
        self.arr = [None]*cap
        self.top = [-1]*k
        self.next = list(range(1,cap)) + [-1]
        self.free = 0 if cap else -1
    def push(self, stack, v):
        if not 0 <= stack < self.k: raise IndexError("Invalid stack")
        if self.free == -1: raise OverflowError("Stack storage is full")
        i = self.free; self.free = self.next[i]
        self.next[i] = self.top[stack]; self.top[stack] = i
        self.arr[i] = v
    def pop(self, stack):
        if not 0 <= stack < self.k: raise IndexError("Invalid stack")
        if self.top[stack] == -1: raise IndexError("Stack is empty")
        i = self.top[stack]; self.top[stack] = self.next[i]
        self.next[i] = self.free; self.free = i
        v = self.arr[i]; self.arr[i] = None
        return v
    def peek(self, stack):
        if self.top[stack] == -1: raise IndexError("Stack is empty")
        return self.arr[self.top[stack]]

if __name__ == "__main__":
    s = KStacks(3,10); s.push(1,7); s.push(1,9); s.push(0,4)
    print(s.pop(1), s.peek(0))
