# GCN From Scratch: Mathematics for Beginners

## What is a GCN?

GCN means **Graph Convolutional Network**.

It is a type of neural network designed for data that looks like a **network**.

A normal neural network is good at data such as:

```text
Student -> marks, attendance, age
House   -> rooms, area, price
Image   -> pixels
```

But many real-world problems look like this:

```text
Person ---- Person
  |           |
  |           |
Person ---- Person
```

or:

```text
Web page ---- Web page ---- Web page
     |
   Web page
```

Here, the important thing is not only the information stored in each object, but also **which objects are connected**.

A GCN learns from both:

- information about each node
- connections between nodes

---

# 1. First understand a graph

A graph has two basic things:

### Nodes

Nodes are the objects.

For example:

```text
0 = Alice
1 = Bob
2 = Charlie
3 = David
```

### Edges

Edges tell us which nodes are connected.

Suppose:

```text
Alice ---- Bob
  |         |
  |         |
Charlie ---- David
```

Then:

- Alice is connected to Bob
- Alice is connected to Charlie
- Bob is connected to David
- Charlie is connected to David

We can represent this using an **adjacency matrix**.

---

# 2. Adjacency matrix

An adjacency matrix is simply a table.

If two nodes are connected, write `1`.

If they are not connected, write `0`.

For example:

```text
      0  1  2  3

0     0  1  1  0
1     1  0  0  1
2     1  0  0  1
3     0  1  1  0
```

So:

```text
A[0][1] = 1
```

means:

> Node 0 is connected to Node 1.

And:

```text
A[0][3] = 0
```

means:

> Node 0 is not connected to Node 3.

---

# 3. Give every node some information

A GCN needs more than connections.

Every node can have features.

For example, suppose we are classifying students.

Each student has:

```text
Math score
Programming score
```

Then:

```text
X =
        Math   Programming

Alice    80       90
Bob      60       70
Charlie  90       95
David    50       60
```

This is called the **feature matrix**.

We can write:

```text
X =
[ 80 90
  60 70
  90 95
  50 60 ]
```

Each row belongs to one node.

---

# 4. Why should a node care about its neighbors?

Imagine Alice is connected to Bob and Charlie.

Alice's own information tells us something about Alice.

But Bob and Charlie may also give useful information.

A GCN says:

> "Let's allow every node to learn from the nodes connected to it."

So Alice can collect information from:

```text
Alice
  +
Bob
  +
Charlie
```

This is called **message passing** or **neighborhood aggregation**.

---

# 5. The first simple idea

Imagine every node simply adds the features of its neighbors.

For Alice:

```text
Alice + Bob + Charlie
```

For Bob:

```text
Bob + Alice + David
```

This already gives us a very simple graph-learning operation.

But there is a problem.

---

# 6. The problem with large neighborhoods

Suppose:

```text
Node A -> 2 neighbors
Node B -> 100 neighbors
```

If we simply add all neighbor information:

```text
A gets information from 2 nodes
B gets information from 100 nodes
```

Node B will receive a much larger amount of information.

That can make learning unstable.

So GCN uses **normalization**.

---

# 7. Self-loops

A node should also remember its own information.

So we connect every node to itself.

This is called adding a **self-loop**.

Mathematically:

\[
\hat A = A + I
\]

where:

- \(A\) = original adjacency matrix
- \(I\) = identity matrix
- \(\hat A\) = adjacency matrix with self-loops

The identity matrix looks like:

```text
1 0 0 0
0 1 0 0
0 0 1 0
0 0 0 1
```

After adding it, every node has a connection to itself.

---

# 8. Degree

The **degree** of a node means:

> How many connections does this node have?

Suppose:

```text
A ---- B
|
C
```

A has two neighbors.

So:

```text
degree(A) = 2
```

After adding self-loops:

```text
degree(A) = 3
```

because A is connected to:

```text
A, B, C
```

---

# 9. Degree matrix

We put the degrees on the diagonal of a matrix.

Suppose the degrees are:

```text
Node 0 -> 3
Node 1 -> 2
Node 2 -> 4
Node 3 -> 2
```

Then:

\[
\hat D =
\begin{bmatrix}
3&0&0&0\\
0&2&0&0\\
0&0&4&0\\
0&0&0&2
\end{bmatrix}
\]

Everything outside the diagonal is zero.

---

# 10. Why do we use square roots?

GCN uses this normalization:

\[
\tilde A =
\hat D^{-1/2}
\hat A
\hat D^{-1/2}
\]

This looks scary.

It is actually just a way of giving each connection a sensible weight.

For a connection between node \(i\) and node \(j\), the weight becomes:

\[
\frac{1}{\sqrt{\hat d_i\hat d_j}}
\]

where:

- \(\hat d_i\) = degree of node \(i\)
- \(\hat d_j\) = degree of node \(j\)

So highly connected nodes do not overwhelm the others.

---

# 11. What does normalization actually mean?

Suppose:

```text
Node A has degree 4
Node B has degree 2
```

Their connection gets weight:

\[
\frac{1}{\sqrt{4\times2}}
\]

\[
=\frac{1}{\sqrt 8}
\]

\[
\approx 0.3536
\]

So instead of simply saying:

```text
"Take all the information!"
```

GCN says:

```text
"Take the information, but give it a controlled weight."
```

That is the main reason for the normalization matrix.

---

# 12. The heart of GCN

Now we reach the famous GCN equation:

\[
H^{(l+1)}
=
\sigma
\left(
\tilde A H^{(l)} W^{(l)}
\right)
\]

Do not try to memorize it.

Let's translate it into normal language.

### \(H^{(l)}\)

Information currently stored by every node.

At the beginning:

\[
H^{(0)}=X
\]

So the first layer starts with the original node features.

### \(\tilde A\)

Tells us:

> "Who should talk to whom, and how strongly?"

### \(W^{(l)}\)

Learnable numbers.

These are the neural network's parameters.

The network changes these numbers during training.

### \(\sigma\)

An activation function.

Usually we use ReLU.

---

# 13. ReLU

ReLU means:

\[
ReLU(x)=\max(0,x)
\]

It is extremely simple.

If the number is positive:

```text
ReLU(5) = 5
```

If the number is negative:

```text
ReLU(-3) = 0
```

So:

```text
Input:   -5   2   -1   7
Output:   0   2    0   7
```

ReLU helps the network learn non-linear patterns.

---

# 14. What does W do?

Suppose a node has:

```text
[80, 90]
```

and the model has learned:

```text
W =
[0.2  0.5
 0.7  0.1]
```

Matrix multiplication changes the representation.

The network can learn combinations such as:

```text
Math is important
Programming is important
Both together are important
```

The values in W are not manually chosen.

They are learned during training.

---

# 15. One GCN layer in plain English

The equation:

\[
H^{(l+1)}
=
\sigma(\tilde A H^{(l)}W^{(l)})
\]

means:

### Step 1

Look at the node's neighbors.

### Step 2

Collect their information.

### Step 3

Normalize that information.

### Step 4

Mix the information using the learned weights.

### Step 5

Apply ReLU.

That produces a new representation for every node.

---

# 16. Two GCN layers

A common GCN has two layers.

First:

\[
H^{(1)}
=
ReLU(\tilde A XW^{(0)})
\]

Second:

\[
Z=
\tilde A H^{(1)}W^{(1)}
\]

Then:

\[
\hat Y=softmax(Z)
\]

So:

```text
Original features
       ↓
   GCN Layer 1
       ↓
     ReLU
       ↓
   GCN Layer 2
       ↓
    Softmax
       ↓
Prediction
```

---

# 17. Why two layers are interesting

Suppose:

```text
A -- B -- C
```

After one GCN layer:

```text
A learns from B
B learns from A and C
C learns from B
```

After a second GCN layer:

```text
A can indirectly learn from C
```

Information has travelled farther through the graph.

So:

> One layer roughly gives one-hop neighborhood information.

> Two layers allow information from roughly two hops away to influence a node.

This is one of the most important ideas in GCNs.

---

# 18. Softmax

At the end, suppose the network produces:

```text
[2.0, 1.0, 0.1]
```

These are called **logits**.

Softmax turns them into probabilities.

For each class:

\[
softmax(z_i)
=
\frac{e^{z_i}}
{\sum_j e^{z_j}}
\]

The results will add up to 1.

For example:

```text
Class A = 0.66
Class B = 0.24
Class C = 0.10
```

The model predicts:

```text
Class A
```

because it has the highest probability.

---

# 19. Training

At the beginning, the weights \(W^{(0)}\) and \(W^{(1)}\) are mostly random.

So predictions are bad.

We need to teach the network.

We give it:

```text
Input graph
+
Correct answers
```

Then we calculate how wrong the prediction is.

This is called the **loss**.

---

# 20. Cross-entropy loss

For classification, a common loss is:

\[
L=
-\sum_i\sum_c
Y_{ic}\log(\hat Y_{ic})
\]

You do not need to fear this equation.

It basically says:

> "If the model gives high probability to the correct answer, the loss is small. If it gives low probability to the correct answer, the loss is large."

For one example:

```text
Correct class probability = 0.9
```

Loss:

\[
-\log(0.9)
\]

which is small.

But:

```text
Correct class probability = 0.1
```

gives:

\[
-\log(0.1)
\]

which is much larger.

---

# 21. Backpropagation

Now the model asks:

> "Which weights caused my mistake?"

Backpropagation calculates this.

It calculates **gradients**.

A gradient tells us:

> "If I change this weight slightly, how does the loss change?"

We calculate:

\[
\frac{\partial L}{\partial W}
\]

---

# 22. Gradient descent

Once we know the gradient, we update the weights.

\[
W_{new}
=
W_{old}
-
\eta
\frac{\partial L}{\partial W}
\]

where:

\[
\eta
\]

is the learning rate.

Think of the loss as a hill.

The model wants to walk downhill.

The gradient tells it which direction is uphill.

So we move in the opposite direction.

---

# 23. Complete GCN learning process

The complete process is:

```text
              GRAPH
                ↓
        Adjacency Matrix A
                ↓
          Add Self Loops
                ↓
             A + I
                ↓
        Calculate Degrees
                ↓
        Normalize Graph
                ↓
              Ã
                ↓
        Node Features X
                ↓
       ┌────────────────┐
       │   GCN Layer 1  │
       │  Ã X W0        │
       │     +          │
       │    ReLU        │
       └────────────────┘
                ↓
              H1
                ↓
       ┌────────────────┐
       │   GCN Layer 2  │
       │  Ã H1 W1       │
       └────────────────┘
                ↓
               Z
                ↓
            Softmax
                ↓
           Prediction
                ↓
             Loss
                ↓
         Backpropagation
                ↓
           Gradients
                ↓
        Gradient Descent
                ↓
        Updated W0, W1
                ↓
             Repeat
```

---

# 24. The most important equation

Remember this:

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

And remember the translation:

> **Neighbors → collect information → normalize → learn a transformation → activate → new node representation.**

That is the core idea of a GCN.

---

# 25. Why GCN is useful

GCNs can be used whenever relationships matter.

Examples:

### Social networks

```text
Person ↔ Person
```

Predict:

- communities
- interests
- connections

### Recommendation systems

```text
User ↔ Product
```

Predict:

- what a user may like

### Molecular graphs

```text
Atom ↔ Atom
```

Predict:

- molecular properties
- toxicity
- chemical behavior

### Citation networks

```text
Paper ↔ Paper
```

Predict:

- paper category
- research area

### Fraud detection

```text
Account ↔ Account
Transaction ↔ Account
```

Detect suspicious structures.

---

# 26. What we are implementing

Our C++ implementation will deliberately expose the mathematics.

We will implement:

```text
Matrix
   ↓
Adjacency matrix
   ↓
Self-loops
   ↓
Degree matrix
   ↓
Normalized adjacency
   ↓
Weight matrices
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
```

The goal is not merely to obtain a prediction.

The goal is to understand **why the prediction happens**.

---

# 27. One-sentence definition

If someone asks:

> What is a GCN?

A good beginner answer is:

> **A Graph Convolutional Network is a neural network that learns the representation of each node by combining its own information with information from its neighboring nodes.**
