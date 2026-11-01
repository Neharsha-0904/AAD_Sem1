def hanoi(n, source, auxiliary, destination):
    if n == 0: return
    hanoi(n-1, source, destination, auxiliary)
    print(f"Move disk {n} from {source} to {destination}")
    hanoi(n-1, auxiliary, source, destination)

if __name__ == "__main__":
    n = 3
    print("Minimum moves:", 2**n - 1)
    hanoi(n, "A", "B", "C")
