/*
    ================================================================
    GCN FROM SCRATCH IN C++
    ================================================================

    Educational implementation of a 2-layer Graph Convolutional
    Network.

    No:
        - PyTorch
        - TensorFlow
        - PyTorch Geometric
        - Eigen

    The implementation exposes the mathematics directly.

    Main equations:

        A_hat = A + I

        A_norm = D^(-1/2) A_hat D^(-1/2)

        H1 = ReLU(A_norm X W0)

        Z  = A_norm H1 W1

        Y_hat = Softmax(Z)

        W = W - learning_rate * dW

    Dataset:
        Small 4-node graph created directly in main().

    Compile:
        g++ -std=c++17 -O2 gcn.cpp -o gcn

    Run:
        ./gcn
*/

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::size_t;
using std::vector;

/* ================================================================
   MATRIX CLASS
   ================================================================ */

class Matrix {
public:
    int rows;
    int cols;
    vector<double> data;

    Matrix() : rows(0), cols(0) {}

    Matrix(int r, int c, double value = 0.0)
        : rows(r), cols(c), data(static_cast<size_t>(r * c), value) {}

    double& operator()(int r, int c) {
        return data[static_cast<size_t>(r * cols + c)];
    }

    double operator()(int r, int c) const {
        return data[static_cast<size_t>(r * cols + c)];
    }

    void fill(double value) {
        std::fill(data.begin(), data.end(), value);
    }

    void print(const std::string& name, int precision = 4) const {
        cout << "\n" << name << " (" << rows << " x " << cols << ")\n";
        cout << std::fixed << std::setprecision(precision);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << std::setw(10) << (*this)(i, j) << " ";
            }
            cout << '\n';
        }
    }
};

/* ================================================================
   BASIC MATRIX OPERATIONS
   ================================================================ */

Matrix matrixMultiply(const Matrix& A, const Matrix& B) {
    if (A.cols != B.rows) {
        throw std::runtime_error(
            "Matrix multiplication dimension mismatch."
        );
    }

    Matrix C(A.rows, B.cols, 0.0);

    for (int i = 0; i < A.rows; ++i) {
        for (int k = 0; k < A.cols; ++k) {
            double a = A(i, k);

            for (int j = 0; j < B.cols; ++j) {
                C(i, j) += a * B(k, j);
            }
        }
    }

    return C;
}

Matrix transpose(const Matrix& A) {
    Matrix T(A.cols, A.rows);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            T(j, i) = A(i, j);
        }
    }

    return T;
}

Matrix add(const Matrix& A, const Matrix& B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        throw std::runtime_error("Matrix addition dimension mismatch.");
    }

    Matrix C(A.rows, A.cols);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            C(i, j) = A(i, j) + B(i, j);
        }
    }

    return C;
}

Matrix subtract(const Matrix& A, const Matrix& B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        throw std::runtime_error("Matrix subtraction dimension mismatch.");
    }

    Matrix C(A.rows, A.cols);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            C(i, j) = A(i, j) - B(i, j);
        }
    }

    return C;
}

Matrix scalarMultiply(const Matrix& A, double scalar) {
    Matrix C(A.rows, A.cols);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            C(i, j) = A(i, j) * scalar;
        }
    }

    return C;
}

Matrix hadamardMultiply(const Matrix& A, const Matrix& B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        throw std::runtime_error(
            "Hadamard multiplication dimension mismatch."
        );
    }

    Matrix C(A.rows, A.cols);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            C(i, j) = A(i, j) * B(i, j);
        }
    }

    return C;
}

/* ================================================================
   MATRIX CONSTRUCTION
   ================================================================ */

Matrix identityMatrix(int n) {
    Matrix I(n, n, 0.0);

    for (int i = 0; i < n; ++i) {
        I(i, i) = 1.0;
    }

    return I;
}

/*
    Create a diagonal matrix from a vector.

    Example:
        values = [3, 4, 2]

        result =
        [3 0 0
         0 4 0
         0 0 2]
*/
Matrix diagonalMatrix(const vector<double>& values) {
    int n = static_cast<int>(values.size());
    Matrix D(n, n, 0.0);

    for (int i = 0; i < n; ++i) {
        D(i, i) = values[i];
    }

    return D;
}

/* ================================================================
   GRAPH NORMALIZATION
   ================================================================ */

/*
    Calculate degree of each node.

    For an adjacency matrix A:

        degree[i] = sum of row i
*/
vector<double> calculateDegrees(const Matrix& A) {
    vector<double> degrees(A.rows, 0.0);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            degrees[i] += A(i, j);
        }
    }

    return degrees;
}

/*
    GCN normalization:

        A_norm = D^(-1/2) A_hat D^(-1/2)

    Because D is diagonal, we can construct D^(-1/2)
    explicitly and use ordinary matrix multiplication.
*/
Matrix normalizeAdjacency(const Matrix& A) {
    if (A.rows != A.cols) {
        throw std::runtime_error(
            "Adjacency matrix must be square."
        );
    }

    // Add self-loops:
    //
    // A_hat = A + I
    Matrix A_hat = add(A, identityMatrix(A.rows));

    vector<double> degrees = calculateDegrees(A_hat);

    vector<double> inverseSqrtDegrees(degrees.size());

    for (size_t i = 0; i < degrees.size(); ++i) {
        if (degrees[i] <= 0.0) {
            throw std::runtime_error(
                "A node has zero degree after adding self-loop."
            );
        }

        inverseSqrtDegrees[i] =
            1.0 / std::sqrt(degrees[i]);
    }

    Matrix D_inv_sqrt =
        diagonalMatrix(inverseSqrtDegrees);

    return matrixMultiply(
        matrixMultiply(D_inv_sqrt, A_hat),
        D_inv_sqrt
    );
}

/* ================================================================
   ACTIVATION FUNCTIONS
   ================================================================ */

/*
    ReLU:

        ReLU(x) = max(0, x)
*/
Matrix relu(const Matrix& A) {
    Matrix R(A.rows, A.cols);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            R(i, j) = std::max(0.0, A(i, j));
        }
    }

    return R;
}

/*
    ReLU derivative:

        1 if x > 0
        0 otherwise

    Important:
    We need the ORIGINAL pre-ReLU matrix.
*/
Matrix reluDerivative(const Matrix& A) {
    Matrix D(A.rows, A.cols);

    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            D(i, j) = (A(i, j) > 0.0) ? 1.0 : 0.0;
        }
    }

    return D;
}

/* ================================================================
   SOFTMAX
   ================================================================ */

/*
    Apply softmax independently to each row.

    Each row represents one node.

    Numerically stable softmax:
    subtract max(logits) before exp().
*/
Matrix softmax(const Matrix& logits) {
    Matrix probabilities(logits.rows, logits.cols);

    for (int i = 0; i < logits.rows; ++i) {

        double maxLogit = logits(i, 0);

        for (int j = 1; j < logits.cols; ++j) {
            maxLogit = std::max(maxLogit, logits(i, j));
        }

        double sumExp = 0.0;

        for (int j = 0; j < logits.cols; ++j) {
            probabilities(i, j) =
                std::exp(logits(i, j) - maxLogit);

            sumExp += probabilities(i, j);
        }

        for (int j = 0; j < logits.cols; ++j) {
            probabilities(i, j) /= sumExp;
        }
    }

    return probabilities;
}

/* ================================================================
   LOSS
   ================================================================ */

/*
    Cross entropy for integer class labels.

    labels[i] = correct class for node i

    L = - average(log(probability of correct class))
*/
double crossEntropy(
    const Matrix& probabilities,
    const vector<int>& labels
) {
    if (probabilities.rows !=
        static_cast<int>(labels.size())) {
        throw std::runtime_error(
            "Number of labels does not match number of nodes."
        );
    }

    const double epsilon = 1e-12;

    double totalLoss = 0.0;

    for (int i = 0; i < probabilities.rows; ++i) {
        int correctClass = labels[i];

        if (correctClass < 0 ||
            correctClass >= probabilities.cols) {
            throw std::runtime_error(
                "Invalid class label."
            );
        }

        double p =
            std::max(probabilities(i, correctClass),
                     epsilon);

        totalLoss -= std::log(p);
    }

    return totalLoss /
           static_cast<double>(probabilities.rows);
}

/* ================================================================
   RANDOM WEIGHT INITIALIZATION
   ================================================================ */

Matrix randomMatrix(
    int rows,
    int cols,
    double minValue,
    double maxValue,
    std::mt19937& generator
) {
    std::uniform_real_distribution<double> distribution(
        minValue,
        maxValue
    );

    Matrix M(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            M(i, j) = distribution(generator);
        }
    }

    return M;
}

/* ================================================================
   GCN MODEL
   ================================================================ */

class GCN {
public:

    Matrix A_norm;

    Matrix W0;
    Matrix W1;

    int inputFeatures;
    int hiddenFeatures;
    int outputClasses;

    double learningRate;

    /*
        Constructor
    */
    GCN(
        const Matrix& normalizedAdjacency,
        int inputFeatures_,
        int hiddenFeatures_,
        int outputClasses_,
        double learningRate_,
        unsigned int seed = 42
    )
        : A_norm(normalizedAdjacency),
          inputFeatures(inputFeatures_),
          hiddenFeatures(hiddenFeatures_),
          outputClasses(outputClasses_),
          learningRate(learningRate_)
    {
        std::mt19937 generator(seed);

        /*
            Small random initialization.

            W0:
                inputFeatures x hiddenFeatures

            W1:
                hiddenFeatures x outputClasses
        */
        W0 = randomMatrix(
            inputFeatures,
            hiddenFeatures,
            -0.5,
            0.5,
            generator
        );

        W1 = randomMatrix(
            hiddenFeatures,
            outputClasses,
            -0.5,
            0.5,
            generator
        );
    }

    /*
        Forward pass.

        We store intermediate values because they are
        needed during backpropagation.
    */
    Matrix forward(
        const Matrix& X,
        Matrix& Z1,
        Matrix& H1,
        Matrix& Z2
    ) const {

        /*
            Z1 = A_norm X W0

            We calculate left-to-right in two operations.
        */
        Matrix AX =
            matrixMultiply(A_norm, X);

        Z1 =
            matrixMultiply(AX, W0);

        /*
            H1 = ReLU(Z1)
        */
        H1 = relu(Z1);

        /*
            Z2 = A_norm H1 W1
        */
        Matrix AH1 =
            matrixMultiply(A_norm, H1);

        Z2 =
            matrixMultiply(AH1, W1);

        /*
            Y_hat = Softmax(Z2)
        */
        return softmax(Z2);
    }

    /*
        Train for a specified number of epochs.
    */
    void train(
        const Matrix& X,
        const vector<int>& labels,
        int epochs,
        int printEvery = 100
    ) {

        for (int epoch = 0; epoch < epochs; ++epoch) {

            /*
                ====================================================
                FORWARD PASS
                ====================================================
            */

            Matrix Z1;
            Matrix H1;
            Matrix Z2;

            Matrix Y_hat =
                forward(X, Z1, H1, Z2);

            /*
                ====================================================
                LOSS
                ====================================================
            */

            double loss =
                crossEntropy(Y_hat, labels);

            /*
                ====================================================
                BACKPROPAGATION
                ====================================================

                For softmax + cross entropy:

                    dZ2 = Y_hat - Y

                We use a one-hot representation of Y.
            */

            Matrix dZ2(
                Y_hat.rows,
                Y_hat.cols,
                0.0
            );

            for (int i = 0; i < Y_hat.rows; ++i) {

                for (int c = 0;
                     c < Y_hat.cols;
                     ++c) {

                    double target =
                        (c == labels[i]) ? 1.0 : 0.0;

                    dZ2(i, c) =
                        Y_hat(i, c) - target;
                }
            }

            /*
                The loss above is averaged over nodes.

                Therefore divide the output gradient
                by number of nodes.
            */
            dZ2 =
                scalarMultiply(
                    dZ2,
                    1.0 /
                    static_cast<double>(X.rows)
                );

            /*
                Z2 = A_norm H1 W1

                Let:

                    AH1 = A_norm H1

                Then:

                    dW1 = AH1^T dZ2
            */

            Matrix AH1 =
                matrixMultiply(A_norm, H1);

            Matrix dW1 =
                matrixMultiply(
                    transpose(AH1),
                    dZ2
                );

            /*
                Gradient flowing back to H1:

                    dH1 =
                        A_norm^T dZ2 W1^T
            */

            Matrix dAH1 =
                matrixMultiply(
                    dZ2,
                    transpose(W1)
                );

            Matrix dH1 =
                matrixMultiply(
                    transpose(A_norm),
                    dAH1
                );

            /*
                H1 = ReLU(Z1)

                Therefore:

                    dZ1 =
                        dH1 * ReLU'(Z1)
            */

            Matrix dReLU =
                reluDerivative(Z1);

            Matrix dZ1 =
                hadamardMultiply(
                    dH1,
                    dReLU
                );

            /*
                Z1 = A_norm X W0

                Let:

                    AX = A_norm X

                Then:

                    dW0 = AX^T dZ1
            */

            Matrix AX =
                matrixMultiply(A_norm, X);

            Matrix dW0 =
                matrixMultiply(
                    transpose(AX),
                    dZ1
                );

            /*
                ====================================================
                GRADIENT DESCENT
                ====================================================
            */

            W0 =
                subtract(
                    W0,
                    scalarMultiply(
                        dW0,
                        learningRate
                    )
                );

            W1 =
                subtract(
                    W1,
                    scalarMultiply(
                        dW1,
                        learningRate
                    )
                );

            /*
                ====================================================
                PRINT TRAINING INFORMATION
                ====================================================
            */

            if (epoch % printEvery == 0 ||
                epoch == epochs - 1) {

                cout
                    << "Epoch "
                    << std::setw(5)
                    << epoch
                    << " | Loss: "
                    << std::fixed
                    << std::setprecision(6)
                    << loss
                    << '\n';
            }
        }
    }

    /*
        Predict class for each node.
    */
    vector<int> predict(
        const Matrix& X
    ) const {

        Matrix Z1;
        Matrix H1;
        Matrix Z2;

        Matrix probabilities =
            forward(X, Z1, H1, Z2);

        vector<int> predictions(
            probabilities.rows
        );

        for (int i = 0;
             i < probabilities.rows;
             ++i) {

            int bestClass = 0;

            for (int c = 1;
                 c < probabilities.cols;
                 ++c) {

                if (probabilities(i, c) >
                    probabilities(i, bestClass)) {

                    bestClass = c;
                }
            }

            predictions[i] = bestClass;
        }

        return predictions;
    }

    /*
        Return probabilities so we can inspect the model.
    */
    Matrix predictProbabilities(
        const Matrix& X
    ) const {

        Matrix Z1;
        Matrix H1;
        Matrix Z2;

        return forward(
            X,
            Z1,
            H1,
            Z2
        );
    }
};

/* ================================================================
   MAIN
   ================================================================ */

int main() {

    cout << "\n";
    cout << "====================================================\n";
    cout << "             GCN FROM SCRATCH IN C++\n";
    cout << "====================================================\n";

    /*
        ============================================================
        STEP 1: CREATE THE GRAPH
        ============================================================

        Graph:

                0 ----- 1
                |       |
                |       |
                2 ----- 3
    */

    const int numNodes = 4;
    const int inputFeatures = 2;
    const int hiddenFeatures = 4;
    const int numClasses = 2;

    Matrix A(numNodes, numNodes, 0.0);

    /*
        Undirected edges:

            0 -- 1
            0 -- 2
            1 -- 3
            2 -- 3
    */

    A(0, 1) = 1.0;
    A(1, 0) = 1.0;

    A(0, 2) = 1.0;
    A(2, 0) = 1.0;

    A(1, 3) = 1.0;
    A(3, 1) = 1.0;

    A(2, 3) = 1.0;
    A(3, 2) = 1.0;

    A.print("Original adjacency matrix A");

    /*
        ============================================================
        STEP 2: CREATE NODE FEATURES
        ============================================================

        Node 0 -> [1, 0]
        Node 1 -> [1, 1]
        Node 2 -> [0, 1]
        Node 3 -> [0, 0]
    */

    Matrix X(numNodes, inputFeatures);

    X(0, 0) = 1.0;
    X(0, 1) = 0.0;

    X(1, 0) = 1.0;
    X(1, 1) = 1.0;

    X(2, 0) = 0.0;
    X(2, 1) = 1.0;

    X(3, 0) = 0.0;
    X(3, 1) = 0.0;

    X.print("Node feature matrix X");

    /*
        ============================================================
        STEP 3: CREATE LABELS
        ============================================================

        Node 0 -> class 0
        Node 1 -> class 1
        Node 2 -> class 0
        Node 3 -> class 1
    */

    vector<int> labels = {
        0,
        1,
        0,
        1
    };

    /*
        ============================================================
        STEP 4: NORMALIZE THE GRAPH
        ============================================================
    */

    Matrix A_norm =
        normalizeAdjacency(A);

    A_norm.print(
        "Normalized adjacency matrix A_norm"
    );

    /*
        ============================================================
        STEP 5: CREATE GCN
        ============================================================

        Architecture:

            Input:  2 features

            Hidden: 4 features

            Output: 2 classes
    */

    double learningRate = 0.05;

    GCN model(
        A_norm,
        inputFeatures,
        hiddenFeatures,
        numClasses,
        learningRate,
        42
    );

    cout << "\nModel configuration:\n";
    cout << "Input features : "
         << inputFeatures << '\n';

    cout << "Hidden features: "
         << hiddenFeatures << '\n';

    cout << "Output classes : "
         << numClasses << '\n';

    cout << "Learning rate  : "
         << learningRate << '\n';

    /*
        ============================================================
        STEP 6: TRAIN
        ============================================================
    */

    cout << "\nTraining...\n\n";

    const int epochs = 1000;

    model.train(
        X,
        labels,
        epochs,
        100
    );

    /*
        ============================================================
        STEP 7: PREDICT
        ============================================================
    */

    Matrix probabilities =
        model.predictProbabilities(X);

    probabilities.print(
        "Final class probabilities",
        6
    );

    vector<int> predictions =
        model.predict(X);

    cout << "\nFinal predictions:\n";

    for (int i = 0;
         i < numNodes;
         ++i) {

        cout
            << "Node "
            << i
            << " | True class: "
            << labels[i]
            << " | Predicted class: "
            << predictions[i]
            << '\n';
    }

    /*
        ============================================================
        FINAL NOTES
        ============================================================

        The entire GCN has now executed:

            A_hat = A + I

            A_norm =
                D^(-1/2)
                A_hat
                D^(-1/2)

            H1 =
                ReLU(
                    A_norm X W0
                )

            Z =
                A_norm H1 W1

            Y_hat =
                Softmax(Z)

            Loss =
                CrossEntropy(Y_hat, Y)

            Backpropagation

            Gradient descent

        Nothing above is delegated to a neural-network framework.
    */

    cout << "\n====================================================\n";
    cout << "                  RUN COMPLETE\n";
    cout << "====================================================\n";

    return 0;
}
