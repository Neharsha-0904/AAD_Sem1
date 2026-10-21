"""Runnable Python reference for kernel-style GCN aggregation; not a GPU DSL."""
import numpy as np

def aggregate_kernel_style(normalized_adjacency, transformed_features, apply_relu=True):
    """Compute H=ReLU(A_hat @ T) using explicit node and feature loops."""
    a, t = np.asarray(normalized_adjacency, float), np.asarray(transformed_features, float)
    if a.ndim != 2 or a.shape[0] != a.shape[1]:
        raise ValueError("normalized_adjacency must be square")
    if t.ndim != 2 or t.shape[0] != a.shape[0]:
        raise ValueError("transformed_features must have one row per node")
    n, f = t.shape
    out = np.zeros((n, f), dtype=float)
    for i in range(n):
        for k in range(f):
            acc = 0.0
            for j in range(n):
                if a[i, j] != 0:
                    acc += a[i, j] * t[j, k]
            out[i, k] = max(0.0, acc) if apply_relu else acc
    return out
