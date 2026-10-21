# Vanilla GCN + Kernel-Style Aggregation

A runnable Python reference project for a two-layer Vanilla Graph Convolutional Network (GCN). It includes the math, explicit aggregation loops, a demo, and tests.

**Scope note:** `src/baby_kernel_style.py` is executable Python that mirrors the aggregation kernel's logic. It is not GPU code for a particular Baby Kernel DSL. The exact Baby Kernel syntax depends on the framework provided by your course, so this repository does not claim GPU compilation or benchmarking.

## Structure
- `src/gcn.py`: graph normalization and two-layer GCN forward pass.
- `src/baby_kernel_style.py`: explicit node/feature aggregation loops.
- `examples/run_demo.py`: runnable end-to-end example.
- `tests/test_gcn.py`: automated correctness tests.
- `docs/kernel_pseudocode.txt`: framework-neutral pseudocode.

## Mathematics
1. Add self-loops: `A_tilde = A + I`
2. Degree matrix: `D_tilde[i,i] = sum_j A_tilde[i,j]`
3. Normalize: `A_hat = D_tilde^(-1/2) A_tilde D_tilde^(-1/2)`
4. Hidden layer: `H1 = ReLU(A_hat @ X @ W0)`
5. Class probabilities: `Z = softmax(A_hat @ H1 @ W1)`

The kernel-style aggregation computes `H1 = ReLU(A_hat @ T)`, where `T = X @ W0`. Conceptually, each parallel worker handles one output element `(node i, feature k)`.

## Setup and run
Python 3.9+ and NumPy are required.

```bash
python -m pip install -r requirements.txt
python examples/run_demo.py
python -m unittest discover -s tests -v
```

## Porting to a real Baby Kernel framework
Run this reference first, port the aggregation in `src/baby_kernel_style.py` to the exact Baby Kernel API required by your course, then compare results against the reference before benchmarking. This repo is runnable now as a Python reference project, but a framework-specific GPU kernel cannot be honestly supplied without knowing the target API.

## Reference
Kipf, T. N., & Welling, M. (2017). *Semi-Supervised Classification with Graph Convolutional Networks*. ICLR. https://arxiv.org/abs/1609.02907
