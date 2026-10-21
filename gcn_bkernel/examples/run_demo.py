from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import numpy as np
from src.gcn import gcn_forward
from src.baby_kernel_style import aggregate_kernel_style

def main():
    # Four-node chain: 0--1--2--3
    a = np.array([[0,1,0,0],[1,0,1,0],[0,1,0,1],[0,0,1,0]], float)
    x = np.array([[1,0],[0,1],[1,1],[0.5,0.5]], float)
    w0 = np.array([[1,-0.5,0.25],[0.5,1,-0.25]], float)
    w1 = np.array([[1,-0.5],[-0.25,0.75],[0.5,0.25]], float)
    r = gcn_forward(a, x, w0, w1)
    h_kernel = aggregate_kernel_style(r["normalized_adjacency"], r["transformed_features"])
    print("Normalized adjacency:\n", np.round(r["normalized_adjacency"], 3))
    print("\nHidden layer H1:\n", np.round(r["hidden"], 3))
    print("\nKernel-style result matches reference:", np.allclose(h_kernel, r["hidden"]))
    print("\nClass probabilities:\n", np.round(r["probabilities"], 3))
    print("\nPredicted class per node:", r["predictions"])

if __name__ == "__main__":
    main()
