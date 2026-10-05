# Graph Convolutional Network From Scratch

A **Graph Convolutional Network (GCN) implemented from scratch in C++ and Java**, without using deep-learning frameworks.

The purpose of this project is not to build the fastest GCN implementation.

The purpose is to understand **what actually happens inside a GCN**.

Instead of treating a GCN as a black box such as:

```text
GCN(input) → prediction
```

this project breaks the complete process into its mathematical and algorithmic components:

```text
Graph
  ↓
Adjacency Matrix
  ↓
Self-Loops
  ↓
Degree Matrix
  ↓
Normalized Adjacency
  ↓
Message Passing
  ↓
Learnable Weights
  ↓
ReLU
  ↓
Softmax
  ↓
Loss
  ↓
Backpropagation
  ↓
Gradient Descent
  ↓
Prediction
```

---

# Why This Project?

Most modern implementations of Graph Neural Networks rely on libraries such as PyTorch Geometric, DGL, TensorFlow, or other optimized frameworks.

Those tools are extremely useful for real applications, but they hide many of the mathematical operations happening underneath.

This project takes the opposite approach.

Every major part of the GCN is implemented explicitly so that the relationship between:

```text
Mathematics
     ↓
Algorithm
     ↓
Code
```

can be seen directly.

For example, the fundamental GCN operation:

\[
H^{(l+1)}
=
\sigma
\left(
\tilde{A}H^{(l)}W^{(l)}
\right)
\]

can be traced directly into the source code.

The project is therefore intended as a **learning and experimentation implementation**, rather than a production framework.

---

# What Is a GCN?

A Graph Convolutional Network is a neural network designed for **graph-structured data**.

Unlike traditional neural networks, a GCN considers both:

1. The features of an object
2. The relationships between objects

For example, in a social network:

```text
Person A ─── Person B
   │             │
   │             │
Person C ─── Person D
```

Each person is a **node**.

A friendship or relationship is an **edge**.

Each person may also have features:

```text
Age
Interests
Location
Activity
etc.
```

A GCN allows a node to learn not only from its own features, but also from the features of its neighboring nodes.

This process is commonly called **message passing** or **neighborhood aggregation**.

---

# Core Idea

The fundamental GCN layer implemented in this project is:

\[
H^{(l+1)}
=
\sigma
\left(
\tilde{A}
H^{(l)}
W^{(l)}
\right)
\]

where:

- \(H^{(l)}\) = node representations at the current layer
- \(\tilde A\) = normalized adjacency matrix
- \(W^{(l)}\) = learnable weight matrix
- \(\sigma\) = activation function such as ReLU

The normalized adjacency matrix is constructed as:

\[
\tilde A
=
\hat D^{-1/2}
\hat A
\hat D^{-1/2}
\]

where:

\[
\hat A=A+I
\]

The addition of \(I\) gives every node a **self-loop**, allowing a node to preserve information about itself while receiving information from its neighbors.

---

# What Does the GCN Actually Learn?

Consider:

```text
A ─── B ─── C
```

If node B has information about itself and nodes A and C, the GCN can aggregate that information.

After one GCN layer:

```text
A ← B
B ← A + B + C
C ← B
```

After another layer, information can travel farther through the graph.

This allows the network to build increasingly meaningful representations of nodes based on their **local graph neighborhoods**.

---

# What Is Implemented?

This project implements a small two-layer GCN.

The forward pass is:

\[
H^{(1)}
=
ReLU
\left(
\tilde A X W^{(0)}
\right)
\]

followed by:

\[
Z
=
\tilde A H^{(1)} W^{(1)}
\]

and:

\[
\hat Y
=
Softmax(Z)
\]

The model is trained using cross-entropy loss and gradient descent.

The implementation also includes the corresponding backward pass for learning the weight matrices.

---

# Repository Structure

```text
GCN-From-Scratch/
│
├── README.md
│
├── README_MATH.md
│
├── README_CODE.md
│
├── gcn.cpp
│
└── GCN.java
```

---

# File Descriptions

## `README.md`

This file.

It provides the overall project description:

- What the project is
- Why it was created
- What a GCN is
- Where GCNs are useful
- Repository structure
- How the implementations relate to the mathematics

---

## `README_MATH.md`

This document focuses entirely on the **mathematical foundation of GCNs**.

It starts from basic concepts and gradually builds the complete algorithm.

Topics include:

- Graphs
- Nodes
- Edges
- Adjacency matrices
- Node features
- Self-loops
- Degree
- Degree matrix
- Normalization
- Message passing
- Matrix multiplication
- ReLU
- Softmax
- Cross-entropy
- Backpropagation
- Gradient descent
- Complete GCN algorithm

The explanation intentionally starts at a beginner-friendly level before introducing the formal equations.

The goal is:

> Understand the mathematics before reading the implementation.

---

## `README_CODE.md`

This document explains **how the mathematical algorithm becomes code**.

It covers:

- Program architecture
- Matrix operations
- Graph representation
- Input representation
- Normalization
- Forward propagation
- Loss calculation
- Backpropagation
- Gradient descent
- Training loop
- Expected input/output
- Compilation
- Experiments
- Applications
- Limitations

It acts as the bridge between:

```text
README_MATH.md
       ↓
README_CODE.md
       ↓
Source Code
```

---

## `gcn.cpp`

The complete GCN implementation in **C++**.

It is written without external machine-learning libraries.

The implementation includes:

```text
Matrix operations
      ↓
Graph construction
      ↓
Adjacency normalization
      ↓
Weight initialization
      ↓
Forward propagation
      ↓
ReLU
      ↓
Softmax
      ↓
Cross entropy
      ↓
Backpropagation
      ↓
Gradient descent
      ↓
Training
      ↓
Prediction
```

The C++ implementation uses explicit loops and matrices so that the underlying calculations are visible.

---

## `GCN.java`

The same GCN algorithm implemented in **Java**.

The purpose is to demonstrate that the algorithm is independent of a particular programming language.

The Java implementation contains the same major components:

```text
Matrix operations
      ↓
Graph representation
      ↓
Normalization
      ↓
GCN Layer 1
      ↓
ReLU
      ↓
GCN Layer 2
      ↓
Softmax
      ↓
Loss
      ↓
Backpropagation
      ↓
Gradient descent
      ↓
Prediction
```

Having both C++ and Java implementations also makes it easier to compare:

```text
Mathematical algorithm
        ↓
C++ implementation

        versus

Mathematical algorithm
        ↓
Java implementation
```

---

# How to Run

## C++

Compile:

```bash
g++ -std=c++17 -O2 gcn.cpp -o gcn
```

Run:

```bash
./gcn
```

---

## Java

Compile:

```bash
javac GCN.java
```

Run:

```bash
java GCN
```

---

# Example Graph

The current implementation uses a small educational graph:

```text
        0 ───── 1
        │       │
        │       │
        2 ───── 3
```

The adjacency matrix is:

```text
0 1 1 0
1 0 0 1
1 0 0 1
0 1 1 0
```

Each node has two features.

Example:

```text
Node 0 → [1, 0]
Node 1 → [1, 1]
Node 2 → [0, 1]
Node 3 → [0, 0]
```

The model predicts one of two classes for each node.

The graph is deliberately small.

The point is not dataset size.

The point is being able to inspect what happens at every stage.

---

# What You Should Observe When Running It

A typical execution follows this pattern:

```text
Original adjacency matrix
        ↓
Normalized adjacency matrix
        ↓
Model configuration
        ↓
Training
        ↓
Epoch 0
Epoch 100
Epoch 200
...
        ↓
Final class probabilities
        ↓
Final predictions
```

The loss should generally decrease during training.

The exact numerical values may vary because the model weights are initialized randomly.

---

# Where Are GCNs Used?

GCNs are useful whenever the **relationships between objects contain useful information**.

## 1. Social Networks

```text
User ─── User
  │        │
  └── User
```

Possible applications:

- Community detection
- User classification
- Relationship prediction
- Recommendation

---

## 2. Recommendation Systems

Graphs can connect:

```text
User ─── Product
User ─── Movie
User ─── Music
```

A GCN can use the relationships between users and items to learn better representations.

Applications include:

- Product recommendation
- Movie recommendation
- Content recommendation
- Personalized ranking

---

## 3. Molecular Graphs

A molecule can be represented as:

```text
Atom ─── Atom ─── Atom
  \                  /
   ───── Atom ─────
```

where:

```text
Node = Atom
Edge = Chemical bond
```

GCNs and related GNNs can be used for:

- Molecular property prediction
- Drug discovery
- Toxicity prediction
- Chemical classification

---

## 4. Fraud Detection

Financial transactions naturally form graphs.

For example:

```text
Account A
    │
Transaction
    │
Account B
    │
Transaction
    │
Account C
```

Graph learning can help identify unusual patterns involving:

- Accounts
- Transactions
- Devices
- Locations
- Merchants

The relationship structure can sometimes reveal patterns that individual records cannot.

---

## 5. Citation Networks

Academic papers can form a graph:

```text
Paper A → Paper B
     ↓
Paper C → Paper D
```

where:

```text
Node = Paper
Edge = Citation
```

Possible applications:

- Research topic classification
- Paper classification
- Citation prediction
- Research recommendation

---

## 6. Knowledge Graphs

Knowledge graphs represent relationships between entities.

For example:

```text
India ── located_in ── Asia

Einstein ── worked_at ── Princeton

Python ── used_for ── Machine Learning
```

Graph neural networks can learn representations of entities using their surrounding relationships.

---

## 7. Traffic and Transportation Networks

Road networks naturally form graphs.

```text
Intersection ─ Road ─ Intersection
       │                    │
      Road                 Road
       │                    │
Intersection ─ Road ─ Intersection
```

Nodes can represent:

- Intersections
- Stations
- Locations

Edges can represent:

- Roads
- Routes
- Connections

Graph neural networks can be used for traffic prediction and related problems.

---

# Why Not Just Use PyTorch Geometric?

For a real project, using an established framework is usually the correct choice.

Libraries provide:

- GPU acceleration
- Sparse matrix operations
- Automatic differentiation
- Mini-batching
- Large graph support
- Optimized kernels
- Production-ready components

This repository intentionally does **not** prioritize those features.

Instead, it prioritizes understanding.

This implementation answers questions such as:

> What exactly does a GCN layer calculate?

> Why do we add self-loops?

> Why do we normalize the adjacency matrix?

> How does a node receive information from its neighbors?

> Where do the trainable parameters occur?

> How does the error travel backward?

> How are the weights updated?

Once those ideas are understood, frameworks become much easier to use intelligently.

---

# Limitations

This is an **educational implementation**.

It currently uses dense matrices.

That means it is not suitable for very large graphs.

For example, a graph with millions of nodes cannot practically use a dense:

```text
1,000,000 × 1,000,000
```

adjacency matrix.

Real GNN systems usually use sparse graph representations such as:

- Edge lists
- CSR
- CSC
- Sparse tensors

They also use optimized CPU/GPU operations.

Other production concerns include:

- Large-scale sampling
- Mini-batching
- GPU acceleration
- Memory optimization
- Distributed training
- Advanced GNN architectures

These are natural next steps after understanding the basic implementation.

---

# Learning Roadmap

The recommended order for studying this repository is:

```text
1. Read README.md
       ↓
2. Read README_MATH.md
       ↓
3. Understand adjacency matrices
       ↓
4. Understand graph normalization
       ↓
5. Understand message passing
       ↓
6. Read README_CODE.md
       ↓
7. Read gcn.cpp
       ↓
8. Compile and execute
       ↓
9. Trace the output
       ↓
10. Read GCN.java
       ↓
11. Compare Java and C++
       ↓
12. Modify the graph
       ↓
13. Modify node features
       ↓
14. Modify learning rate
       ↓
15. Modify hidden-layer size
       ↓
16. Experiment with the algorithm
```

---

# Suggested Experiments

Once the basic implementation works, modify one thing at a time.

### Experiment 1: Remove self-loops

Compare:

\[
\hat A=A+I
\]

with:

\[
\hat A=A
\]

Ask:

> What happens when a node cannot directly preserve its own information?

---

### Experiment 2: Change the graph

Add or remove edges.

For example:

```text
0 ───── 1
│ \     │
│  \    │
2 ───── 3
```

Observe how changing relationships affects the predictions.

---

### Experiment 3: Change node features

Modify the feature matrix.

Observe how the predictions change.

---

### Experiment 4: Change learning rate

Try:

```text
0.001
0.01
0.05
0.1
```

Observe the effect on convergence.

---

### Experiment 5: Change hidden dimensions

Try:

```text
2
4
8
16
```

Observe how model capacity affects learning.

---

# The Main Idea

The most important thing to take away from this project is:

```text
A GCN does not look at nodes in isolation.

It learns from:

        Node information
              +
        Graph structure
              +
        Learnable parameters
```

Or more formally:

\[
\boxed{
H^{(l+1)}
=
\sigma
\left(
\tilde A H^{(l)}W^{(l)}
\right)
}
\]

The equation is the heart of the project.

The C++ and Java programs are simply two ways of turning that equation into an executable algorithm.

---

# Future Extensions

Possible extensions to this repository include:

- Sparse adjacency matrices
- Cora dataset
- Real-world graph datasets
- Multi-layer GCN
- GraphSAGE
- Graph Attention Networks
- Node embeddings
- Link prediction
- Graph classification
- GPU implementation
- CUDA-based message passing
- Mini-batch training
- Comparing GCN with traditional ML methods

The long-term goal can be summarized as:

```text
Understand GCN
      ↓
Implement GCN
      ↓
Experiment with GCN
      ↓
Use real graph datasets
      ↓
Optimize GCN
      ↓
Build larger GNN systems
```

---

# License

This project is intended primarily for educational and research learning purposes.

Feel free to study, modify, extend, and experiment with the implementation.
