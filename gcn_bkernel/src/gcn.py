"""NumPy reference implementation of a two-layer Vanilla GCN."""
import numpy as np

def normalize_adjacency(adjacency):
    """Return D^(-1/2) (A + I) D^(-1/2)."""
    a = np.asarray(adjacency, dtype=float)
    if a.ndim != 2 or a.shape[0] != a.shape[1]:
        raise ValueError("adjacency must be a square matrix")
    if np.any(a < 0):
        raise ValueError("adjacency weights must be non-negative")
    a_tilde = a + np.eye(a.shape[0])
    inv_sqrt = a_tilde.sum(axis=1) ** -0.5
    return inv_sqrt[:, None] * a_tilde * inv_sqrt[None, :]

def relu(x):
    return np.maximum(x, 0.0)

def softmax(x):
    x = np.asarray(x, dtype=float)
    e = np.exp(x - np.max(x, axis=1, keepdims=True))
    return e / e.sum(axis=1, keepdims=True)

def gcn_forward(adjacency, features, weight_0, weight_1):
    """Compute H1=ReLU(A_hat X W0), Z=softmax(A_hat H1 W1)."""
    a, x = np.asarray(adjacency, float), np.asarray(features, float)
    w0, w1 = np.asarray(weight_0, float), np.asarray(weight_1, float)
    if x.ndim != 2 or a.shape != (x.shape[0], x.shape[0]):
        raise ValueError("adjacency and features have incompatible shapes")
    if w0.ndim != 2 or x.shape[1] != w0.shape[0]:
        raise ValueError("weight_0 must be (input_features, hidden_features)")
    if w1.ndim != 2 or w0.shape[1] != w1.shape[0]:
        raise ValueError("weight_1 must be (hidden_features, classes)")
    ah = normalize_adjacency(a)
    transformed = x @ w0
    hidden = relu(ah @ transformed)
    logits = ah @ hidden @ w1
    probs = softmax(logits)
    return {"normalized_adjacency": ah, "transformed_features": transformed,
            "hidden": hidden, "logits": logits, "probabilities": probs,
            "predictions": np.argmax(probs, axis=1)}
