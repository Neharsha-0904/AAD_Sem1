# GCN From Scratch in C++

## Purpose

This project implements a **Graph Convolutional Network from scratch in C++**.

The purpose is educational:

- understand the mathematics
- understand matrix operations
- understand message passing
- understand forward propagation
- understand loss
- understand backpropagation
- understand gradient descent
- run the complete algorithm without a deep-learning framework

We intentionally avoid:

- PyTorch
- TensorFlow
- PyTorch Geometric
- DGL
- Eigen

The first implementation uses ordinary C++ containers and explicit loops.

---

# 1. What the program does

The program implements a small two-layer GCN:

\[
H^{(1)}
=
ReLU(\tilde A X W^{(0)})
\]

\[
Z
=
\tilde A H^{(1)} W^{(1)}
\]

\[
\hat Y
=
softmax(Z)
\]

During training it calculates the cross-entropy loss, performs backpropagation, and updates the two weight matrices using gradient descent.

---

# 2. Program pipeline

```text
Input Graph
     |
     +---- Adjacency Matrix A
     |
     +---- Node Features X
     |
     +---- Labels Y
     |
     v
Add self loops
     |
     v
Calculate degree
     |
     v
Create normalized adjacency Ã
     |
     v
Initialize W0 and W1
     |
     v
Forward propagation
     |
     v
Prediction
     |
     v
Loss
     |
     v
Backpropagation
     |
     v
Update W0 and W1
     |
     v
Repeat
```

---

# 3. Input representation

For the educational example, we use a small graph with four nodes.

```text
0 ----- 1
|       |
|       |
2 ----- 3
```

The adjacency matrix is:

```text
0 1 1 0
1 0 0 1
1 0 0 1
0 1 1 0
```

The graph is undirected.

Therefore:

```text
A[i][j] == A[j][i]
```

---

# 4. Node features

Each node has two features.

Example:

```text
Node 0: [1.0, 0.0]
Node 1: [1.0, 1.0]
Node 2: [0.0, 1.0]
Node 3: [0.0, 0.0]
```

So:

```text
X =
1 0
1 1
0 1
0 0
```

The dimensions are:

```text
X = 4 x 2
```

Meaning:

```text
4 nodes
2 features per node
```

---

# 5. Labels

Suppose there are two classes:

```text
Class 0
Class 1
```

For example:

```text
Node 0 -> class 0
Node 1 -> class 1
Node 2 -> class 0
Node 3 -> class 1
```

The labels can be represented internally as class IDs:

```text
0
1
0
1
```

---

# 6. Step 1: Add self-loops

The code creates:

\[
\hat A=A+I
\]

For the example:

```text
A =
0 1 1 0
1 0 0 1
1 0 0 1
0 1 1 0
```

The identity matrix is:

```text
1 0 0 0
0 1 0 0
0 0 1 0
0 0 0 1
```

Therefore:

```text
A_hat =
1 1 1 0
1 1 0 1
1 0 1 1
0 1 1 1
```

---

# 7. Step 2: Calculate degrees

For each row we count the number of connections.

Every node in this example has degree 3 after adding the self-loop.

Therefore:

```text
D =
3 0 0 0
0 3 0 0
0 0 3 0
0 0 0 3
```

---

# 8. Step 3: Normalize the adjacency matrix

GCN uses:

\[
\tilde A
=
D^{-1/2}A_{hat}D^{-1/2}
\]

Because all degrees are 3 in this example:

\[
D^{-1/2}
=
\frac{1}{\sqrt3}I
\]

Therefore each existing connection receives:

\[
\frac{1}{3}
\]

and:

```text
A_normalized =
0.3333 0.3333 0.3333 0
0.3333 0.3333 0 0.3333
0.3333 0 0.3333 0.3333
0 0.3333 0.3333 0.3333
```

For a graph with different degrees, the code calculates each weight individually.

---

# 9. Matrix multiplication

The GCN depends heavily on matrix multiplication.

If:

```text
A = m x n
B = n x p
```

then:

```text
A * B = m x p
```

The program implements multiplication manually.

Conceptually:

```cpp
result[i][j] += A[i][k] * B[k][j];
```

This one operation appears repeatedly throughout the GCN.

---

# 10. First GCN layer

The first layer calculates:

\[
Z^{(1)}
=
\tilde A X W^{(0)}
\]

Then:

\[
H^{(1)}
=
ReLU(Z^{(1)})
\]

Suppose:

```text
X = 4 x 2
W0 = 2 x 4
```

Then:

```text
A_normalized * X = 4 x 2

(4 x 2) * (2 x 4)
= 4 x 4
```

So the hidden representation has four features per node.

---

# 11. Why matrix multiplication represents message passing

Look at:

```text
A_normalized * X
```

Each row of the normalized adjacency matrix tells us how much information a node receives from other nodes.

For example:

```text
row 0:
0.3333 0.3333 0.3333 0
```

means:

```text
Node 0 receives:

1/3 from itself
1/3 from node 1
1/3 from node 2
0 from node 3
```

This is the mathematical form of neighborhood aggregation.

---

# 12. ReLU

After the first matrix transformation:

```cpp
H1 = relu(Z1);
```

The function is:

```text
relu(x) = max(0, x)
```

Example:

```text
[-1.2, 0.5, -0.3, 2.0]
```

becomes:

```text
[0, 0.5, 0, 2.0]
```

---

# 13. Second GCN layer

The second layer calculates:

\[
Z
=
\tilde A H^{(1)}W^{(1)}
\]

If there are two classes:

```text
W1 = hidden_features x 2
```

Then:

```text
Z = number_of_nodes x 2
```

Each row contains the two class scores for one node.

---

# 14. Softmax

The output scores are converted into probabilities.

For one node:

```text
logits = [2.0, 1.0]
```

Softmax produces approximately:

```text
[0.7311, 0.2689]
```

The predicted class is:

```text
0
```

because it has the larger probability.

---

# 15. Loss

We use cross entropy.

For one node:

\[
L=-\log(p_{correct})
\]

If the correct class probability is:

```text
0.9
```

the loss is small.

If it is:

```text
0.1
```

the loss is large.

The training process tries to make this loss smaller.

---

# 16. Backpropagation

The program calculates how each weight contributed to the error.

For the softmax + cross entropy combination:

\[
\frac{\partial L}{\partial Z}
=
\hat Y-Y
\]

This is especially useful because the gradient becomes simple.

For the second weight matrix:

\[
\frac{\partial L}{\partial W^{(1)}}
=
(H^{(1)})^T
\tilde A^T
\frac{\partial L}{\partial Z}
\]

Then the gradient is propagated into the hidden layer.

---

# 17. Gradient descent

The weights are updated using:

\[
W_{new}
=
W_{old}
-
\eta
\frac{\partial L}{\partial W}
\]

where:

```text
eta = learning rate
```

Example:

```text
learning_rate = 0.01
```

The code performs:

```cpp
W0 = W0 - learning_rate * dW0;
W1 = W1 - learning_rate * dW1;
```

---

# 18. Training loop

The main training process looks like:

```cpp
for (int epoch = 0; epoch < epochs; ++epoch) {

    // Forward pass
    ...

    // Calculate loss
    ...

    // Backward pass
    ...

    // Update parameters
    ...

}
```

The same process happens repeatedly.

A typical run may look like:

```text
Epoch 0    Loss: 0.72
Epoch 100  Loss: 0.55
Epoch 200  Loss: 0.41
Epoch 300  Loss: 0.31
Epoch 400  Loss: 0.24
```

The exact values depend on initialization and the dataset.

The important observation is that the loss should generally decrease.

---

# 19. Expected output

A successful educational run should print information similar to:

```text
===== GCN FROM SCRATCH =====

Graph:
Nodes: 4
Features per node: 2
Classes: 2

Normalized adjacency:
0.3333 0.3333 0.3333 0.0000
0.3333 0.3333 0.0000 0.3333
0.3333 0.0000 0.3333 0.3333
0.0000 0.3333 0.3333 0.3333

Training...

Epoch 0    Loss: ...
Epoch 100  Loss: ...
Epoch 200  Loss: ...
...

Final predictions:
Node 0 -> Class ...
Node 1 -> Class ...
Node 2 -> Class ...
Node 3 -> Class ...
```

The exact numerical output can differ because the weights are randomly initialized.

---

# 20. Compile

With a standard C++ compiler:

```bash
g++ -std=c++17 -O2 gcn.cpp -o gcn
```

Then:

```bash
./gcn
```

On Windows with MinGW:

```bash
g++ -std=c++17 -O2 gcn.cpp -o gcn.exe
gcn.exe
```

---

# 21. Recommended first execution

Do not immediately modify the program.

First run:

```bash
g++ -std=c++17 -O2 gcn.cpp -o gcn
./gcn
```

Confirm that:

1. the graph is created
2. self-loops are added
3. normalized adjacency is printed
4. training starts
5. loss is printed
6. predictions are printed

Then change one component at a time.

---

# 22. Experiments to perform

## Experiment 1: Remove self-loops

Change:

```text
A_hat = A + I
```

to:

```text
A_hat = A
```

Observe what changes.

Question:

> Why should a node remember its own features?

---

## Experiment 2: Change the learning rate

Try:

```text
0.001
0.01
0.1
```

Observe the loss.

Question:

> What happens when the learning rate is too small or too large?

---

## Experiment 3: Change the number of hidden features

Try:

```text
2
4
8
16
```

Observe the learning behavior.

---

## Experiment 4: Change the graph

Add an edge:

```text
0 ---- 3
```

Then run again.

Observe how changing the graph changes the predictions.

---

## Experiment 5: Change node features

Modify:

```text
Node 0: [1,0]
```

to:

```text
Node 0: [0,1]
```

Run again.

Observe the effect.

---

# 23. Where this implementation can be used

This implementation is best used for:

- learning GCN mathematics
- university assignments
- teaching graph neural networks
- understanding backpropagation
- experimenting with graph structures
- validating equations
- creating a foundation for larger GNN implementations

It is **not intended to compete with production GNN frameworks**.

For large graphs, production systems use optimized:

- sparse matrices
- GPU kernels
- batching
- automatic differentiation
- memory-efficient graph storage

---

# 24. Where GCNs are used in the real world

The same basic idea can be applied to:

### Social networks

```text
user -> user
```

### Recommendation systems

```text
user -> product
```

### Molecular analysis

```text
atom -> atom
```

### Fraud detection

```text
account -> transaction -> account
```

### Citation networks

```text
paper -> paper
```

### Knowledge graphs

```text
entity -> relation -> entity
```

---

# 25. Important limitation of this educational code

This project uses **dense matrices**.

If there are:

```text
1,000,000 nodes
```

a dense adjacency matrix would require an enormous amount of memory.

Real GCN systems usually store graphs sparsely:

```text
edge list
CSR
CSC
sparse tensors
```

For example, instead of storing:

```text
1,000,000 x 1,000,000
```

mostly-zero entries, we store only the existing edges.

That is a major next step after understanding this implementation.

---

# 26. Suggested learning order

Study the code in this order:

```text
1. Matrix class
2. Matrix multiplication
3. Adjacency matrix
4. Self-loops
5. Degree calculation
6. Normalization
7. Random weight initialization
8. ReLU
9. Softmax
10. Forward propagation
11. Cross entropy
12. Backpropagation
13. Gradient descent
14. Training loop
15. Prediction
```

Do not skip directly to the training loop.

The training loop is only a small wrapper around the mathematics.

---

# 27. The complete algorithm in one place

```text
INPUT:
    Graph A
    Node features X
    Labels Y

1. A_hat = A + I

2. Calculate degree of every node

3. Create:
       D_hat^(-1/2)

4. Calculate:
       A_norm =
       D_hat^(-1/2)
       A_hat
       D_hat^(-1/2)

5. Initialize:
       W0
       W1

6. Repeat for each epoch:

       H1 = ReLU(A_norm X W0)

       Z = A_norm H1 W1

       Y_pred = Softmax(Z)

       Loss = CrossEntropy(Y_pred, Y)

       Calculate gradients

       W0 = W0 - learning_rate * dW0

       W1 = W1 - learning_rate * dW1

7. Predict:
       argmax(Y_pred)

OUTPUT:
       predicted class for each node
```

---

# 28. Project philosophy

The most important objective is:

> **Do not treat the GCN as a black box.**

Every important line of code should correspond to a mathematical operation.

For example:

```cpp
A_hat = add(A, identity);
```

corresponds to:

\[
\hat A=A+I
\]

and:

```cpp
H1 = relu(matmul(matmul(A_norm, X), W0));
```

corresponds to:

\[
H^{(1)}
=
ReLU(\tilde A X W^{(0)})
\]

This one-to-one connection between **equation → algorithm → C++ code** is the main purpose of the project.
