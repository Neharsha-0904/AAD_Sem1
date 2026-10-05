/*
 * ================================================================
 * GCN FROM SCRATCH IN JAVA
 * ================================================================
 *
 * Educational implementation of a 2-layer Graph Convolutional
 * Network without external ML libraries.
 *
 * Main equations:
 *
 *   A_hat = A + I
 *
 *   A_norm = D^(-1/2) A_hat D^(-1/2)
 *
 *   H1 = ReLU(A_norm X W0)
 *
 *   Z  = A_norm H1 W1
 *
 *   Y_hat = Softmax(Z)
 *
 *   W = W - learningRate * dW
 *
 * Compile:
 *   javac GCN.java
 *
 * Run:
 *   java GCN
 */

import java.util.Arrays;
import java.util.Random;

public class GCN {

    /* ============================================================
       MATRIX CLASS
       ============================================================ */

    static class Matrix {
        int rows;
        int cols;
        double[][] data;

        Matrix(int rows, int cols) {
            this.rows = rows;
            this.cols = cols;
            this.data = new double[rows][cols];
        }

        Matrix(int rows, int cols, double value) {
            this(rows, cols);

            for (int i = 0; i < rows; i++) {
                Arrays.fill(this.data[i], value);
            }
        }

        void print(String name) {
            print(name, 4);
        }

        void print(String name, int precision) {
            System.out.println(
                "\n" + name +
                " (" + rows + " x " + cols + ")"
            );

            String format = "%" + (precision + 7) +
                            "." + precision + "f ";

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    System.out.printf(
                        format,
                        data[i][j]
                    );
                }
                System.out.println();
            }
        }
    }

    /* ============================================================
       BASIC MATRIX OPERATIONS
       ============================================================ */

    static Matrix multiply(Matrix A, Matrix B) {

        if (A.cols != B.rows) {
            throw new IllegalArgumentException(
                "Matrix multiplication dimension mismatch."
            );
        }

        Matrix C = new Matrix(A.rows, B.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int k = 0; k < A.cols; k++) {

                double a = A.data[i][k];

                for (int j = 0; j < B.cols; j++) {
                    C.data[i][j] +=
                        a * B.data[k][j];
                }
            }
        }

        return C;
    }

    static Matrix transpose(Matrix A) {

        Matrix T = new Matrix(
            A.cols,
            A.rows
        );

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                T.data[j][i] = A.data[i][j];
            }
        }

        return T;
    }

    static Matrix add(Matrix A, Matrix B) {

        checkSameShape(A, B);

        Matrix C =
            new Matrix(A.rows, A.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                C.data[i][j] =
                    A.data[i][j] +
                    B.data[i][j];
            }
        }

        return C;
    }

    static Matrix subtract(Matrix A, Matrix B) {

        checkSameShape(A, B);

        Matrix C =
            new Matrix(A.rows, A.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                C.data[i][j] =
                    A.data[i][j] -
                    B.data[i][j];
            }
        }

        return C;
    }

    static Matrix scalarMultiply(
        Matrix A,
        double scalar
    ) {

        Matrix C =
            new Matrix(A.rows, A.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                C.data[i][j] =
                    A.data[i][j] * scalar;
            }
        }

        return C;
    }

    static Matrix hadamardMultiply(
        Matrix A,
        Matrix B
    ) {

        checkSameShape(A, B);

        Matrix C =
            new Matrix(A.rows, A.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                C.data[i][j] =
                    A.data[i][j] *
                    B.data[i][j];
            }
        }

        return C;
    }

    static void checkSameShape(
        Matrix A,
        Matrix B
    ) {

        if (A.rows != B.rows ||
            A.cols != B.cols) {

            throw new IllegalArgumentException(
                "Matrix dimensions do not match."
            );
        }
    }

    /* ============================================================
       MATRIX CONSTRUCTION
       ============================================================ */

    static Matrix identity(int n) {

        Matrix I = new Matrix(n, n);

        for (int i = 0; i < n; i++) {
            I.data[i][i] = 1.0;
        }

        return I;
    }

    static Matrix diagonal(
        double[] values
    ) {

        Matrix D =
            new Matrix(
                values.length,
                values.length
            );

        for (int i = 0; i < values.length; i++) {
            D.data[i][i] = values[i];
        }

        return D;
    }

    /* ============================================================
       GRAPH NORMALIZATION
       ============================================================ */

    static double[] degrees(Matrix A) {

        double[] degree =
            new double[A.rows];

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                degree[i] += A.data[i][j];
            }
        }

        return degree;
    }

    /*
     * A_norm = D^(-1/2) (A + I) D^(-1/2)
     */
    static Matrix normalizeAdjacency(Matrix A) {

        if (A.rows != A.cols) {
            throw new IllegalArgumentException(
                "Adjacency matrix must be square."
            );
        }

        // A_hat = A + I
        Matrix A_hat =
            add(A, identity(A.rows));

        double[] degree =
            degrees(A_hat);

        double[] inverseSqrt =
            new double[degree.length];

        for (int i = 0; i < degree.length; i++) {

            if (degree[i] <= 0) {
                throw new IllegalArgumentException(
                    "Node has zero degree."
                );
            }

            inverseSqrt[i] =
                1.0 /
                Math.sqrt(degree[i]);
        }

        Matrix D_inv_sqrt =
            diagonal(inverseSqrt);

        return multiply(
            multiply(
                D_inv_sqrt,
                A_hat
            ),
            D_inv_sqrt
        );
    }

    /* ============================================================
       ACTIVATION FUNCTIONS
       ============================================================ */

    static Matrix relu(Matrix A) {

        Matrix R =
            new Matrix(A.rows, A.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                R.data[i][j] =
                    Math.max(0.0, A.data[i][j]);
            }
        }

        return R;
    }

    static Matrix reluDerivative(Matrix A) {

        Matrix D =
            new Matrix(A.rows, A.cols);

        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < A.cols; j++) {
                D.data[i][j] =
                    A.data[i][j] > 0.0
                    ? 1.0
                    : 0.0;
            }
        }

        return D;
    }

    /* ============================================================
       SOFTMAX
       ============================================================ */

    static Matrix softmax(Matrix logits) {

        Matrix probabilities =
            new Matrix(
                logits.rows,
                logits.cols
            );

        for (int i = 0; i < logits.rows; i++) {

            double maxLogit =
                logits.data[i][0];

            for (int j = 1;
                 j < logits.cols;
                 j++) {

                maxLogit =
                    Math.max(
                        maxLogit,
                        logits.data[i][j]
                    );
            }

            double sumExp = 0.0;

            for (int j = 0;
                 j < logits.cols;
                 j++) {

                probabilities.data[i][j] =
                    Math.exp(
                        logits.data[i][j]
                        - maxLogit
                    );

                sumExp +=
                    probabilities.data[i][j];
            }

            for (int j = 0;
                 j < logits.cols;
                 j++) {

                probabilities.data[i][j] /=
                    sumExp;
            }
        }

        return probabilities;
    }

    /* ============================================================
       CROSS ENTROPY
       ============================================================ */

    static double crossEntropy(
        Matrix probabilities,
        int[] labels
    ) {

        if (probabilities.rows !=
            labels.length) {

            throw new IllegalArgumentException(
                "Number of labels does not match nodes."
            );
        }

        double epsilon = 1e-12;
        double totalLoss = 0.0;

        for (int i = 0;
             i < probabilities.rows;
             i++) {

            int correctClass = labels[i];

            if (correctClass < 0 ||
                correctClass >=
                probabilities.cols) {

                throw new IllegalArgumentException(
                    "Invalid class label."
                );
            }

            double p =
                Math.max(
                    probabilities.data[i][correctClass],
                    epsilon
                );

            totalLoss -= Math.log(p);
        }

        return totalLoss /
            probabilities.rows;
    }

    /* ============================================================
       RANDOM INITIALIZATION
       ============================================================ */

    static Matrix randomMatrix(
        int rows,
        int cols,
        double min,
        double max,
        Random random
    ) {

        Matrix M =
            new Matrix(rows, cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {

                M.data[i][j] =
                    min +
                    random.nextDouble()
                    * (max - min);
            }
        }

        return M;
    }

    /* ============================================================
       GCN MODEL
       ============================================================ */

    static class GCNModel {

        Matrix A_norm;

        Matrix W0;
        Matrix W1;

        double learningRate;

        int inputFeatures;
        int hiddenFeatures;
        int outputClasses;

        GCNModel(
            Matrix normalizedAdjacency,
            int inputFeatures,
            int hiddenFeatures,
            int outputClasses,
            double learningRate,
            long seed
        ) {

            this.A_norm =
                normalizedAdjacency;

            this.inputFeatures =
                inputFeatures;

            this.hiddenFeatures =
                hiddenFeatures;

            this.outputClasses =
                outputClasses;

            this.learningRate =
                learningRate;

            Random random =
                new Random(seed);

            W0 =
                randomMatrix(
                    inputFeatures,
                    hiddenFeatures,
                    -0.5,
                    0.5,
                    random
                );

            W1 =
                randomMatrix(
                    hiddenFeatures,
                    outputClasses,
                    -0.5,
                    0.5,
                    random
                );
        }

        /*
         * Forward propagation:
         *
         * H1 = ReLU(A_norm X W0)
         *
         * Z  = A_norm H1 W1
         *
         * Y  = Softmax(Z)
         */
        Matrix forward(
            Matrix X,
            MatrixHolder Z1Holder,
            MatrixHolder H1Holder,
            MatrixHolder Z2Holder
        ) {

            Matrix AX =
                multiply(
                    A_norm,
                    X
                );

            Matrix Z1 =
                multiply(
                    AX,
                    W0
                );

            Matrix H1 =
                relu(Z1);

            Matrix AH1 =
                multiply(
                    A_norm,
                    H1
                );

            Matrix Z2 =
                multiply(
                    AH1,
                    W1
                );

            Matrix Y_hat =
                softmax(Z2);

            Z1Holder.value = Z1;
            H1Holder.value = H1;
            Z2Holder.value = Z2;

            return Y_hat;
        }

        void train(
            Matrix X,
            int[] labels,
            int epochs,
            int printEvery
        ) {

            for (int epoch = 0;
                 epoch < epochs;
                 epoch++) {

                /*
                 * =================================================
                 * FORWARD
                 * =================================================
                 */

                MatrixHolder Z1 =
                    new MatrixHolder();

                MatrixHolder H1 =
                    new MatrixHolder();

                MatrixHolder Z2 =
                    new MatrixHolder();

                Matrix Y_hat =
                    forward(
                        X,
                        Z1,
                        H1,
                        Z2
                    );

                /*
                 * =================================================
                 * LOSS
                 * =================================================
                 */

                double loss =
                    crossEntropy(
                        Y_hat,
                        labels
                    );

                /*
                 * =================================================
                 * dZ2 = Y_hat - Y
                 * =================================================
                 */

                Matrix dZ2 =
                    new Matrix(
                        Y_hat.rows,
                        Y_hat.cols
                    );

                for (int i = 0;
                     i < Y_hat.rows;
                     i++) {

                    for (int c = 0;
                         c < Y_hat.cols;
                         c++) {

                        double target =
                            c == labels[i]
                            ? 1.0
                            : 0.0;

                        dZ2.data[i][c] =
                            Y_hat.data[i][c]
                            - target;
                    }
                }

                /*
                 * Because the loss is averaged over nodes.
                 */
                dZ2 =
                    scalarMultiply(
                        dZ2,
                        1.0 / X.rows
                    );

                /*
                 * =================================================
                 * dW1
                 *
                 * Z2 = A_norm H1 W1
                 *
                 * AH1 = A_norm H1
                 *
                 * dW1 = AH1^T dZ2
                 * =================================================
                 */

                Matrix AH1 =
                    multiply(
                        A_norm,
                        H1.value
                    );

                Matrix dW1 =
                    multiply(
                        transpose(AH1),
                        dZ2
                    );

                /*
                 * =================================================
                 * dH1
                 *
                 * dH1 =
                 * A_norm^T dZ2 W1^T
                 * =================================================
                 */

                Matrix dAH1 =
                    multiply(
                        dZ2,
                        transpose(W1)
                    );

                Matrix dH1 =
                    multiply(
                        transpose(A_norm),
                        dAH1
                    );

                /*
                 * =================================================
                 * dZ1
                 *
                 * H1 = ReLU(Z1)
                 *
                 * dZ1 =
                 * dH1 * ReLU'(Z1)
                 * =================================================
                 */

                Matrix dRelu =
                    reluDerivative(
                        Z1.value
                    );

                Matrix dZ1 =
                    hadamardMultiply(
                        dH1,
                        dRelu
                    );

                /*
                 * =================================================
                 * dW0
                 *
                 * Z1 = A_norm X W0
                 *
                 * AX = A_norm X
                 *
                 * dW0 = AX^T dZ1
                 * =================================================
                 */

                Matrix AX =
                    multiply(
                        A_norm,
                        X
                    );

                Matrix dW0 =
                    multiply(
                        transpose(AX),
                        dZ1
                    );

                /*
                 * =================================================
                 * GRADIENT DESCENT
                 * =================================================
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

                if (epoch % printEvery == 0 ||
                    epoch == epochs - 1) {

                    System.out.printf(
                        "Epoch %5d | Loss: %.6f%n",
                        epoch,
                        loss
                    );
                }
            }
        }

        int[] predict(Matrix X) {

            MatrixHolder Z1 =
                new MatrixHolder();

            MatrixHolder H1 =
                new MatrixHolder();

            MatrixHolder Z2 =
                new MatrixHolder();

            Matrix probabilities =
                forward(
                    X,
                    Z1,
                    H1,
                    Z2
                );

            int[] predictions =
                new int[probabilities.rows];

            for (int i = 0;
                 i < probabilities.rows;
                 i++) {

                int bestClass = 0;

                for (int c = 1;
                     c < probabilities.cols;
                     c++) {

                    if (probabilities.data[i][c]
                        >
                        probabilities.data[i][bestClass]) {

                        bestClass = c;
                    }
                }

                predictions[i] =
                    bestClass;
            }

            return predictions;
        }

        Matrix predictProbabilities(
            Matrix X
        ) {

            MatrixHolder Z1 =
                new MatrixHolder();

            MatrixHolder H1 =
                new MatrixHolder();

            MatrixHolder Z2 =
                new MatrixHolder();

            return forward(
                X,
                Z1,
                H1,
                Z2
            );
        }
    }

    /*
     * Java does not return multiple matrices directly from a
     * method, so these tiny holders let forward() return the
     * prediction while exposing intermediate matrices for
     * backpropagation.
     */
    static class MatrixHolder {
        Matrix value;
    }

    /* ============================================================
       MAIN
       ============================================================ */

    public static void main(String[] args) {

        System.out.println(
            "\n===================================================="
        );

        System.out.println(
            "             GCN FROM SCRATCH IN JAVA"
        );

        System.out.println(
            "===================================================="
        );

        /*
         * ----------------------------------------------------------
         * STEP 1: GRAPH
         *
         *       0 ----- 1
         *       |       |
         *       |       |
         *       2 ----- 3
         * ----------------------------------------------------------
         */

        int numNodes = 4;
        int inputFeatures = 2;
        int hiddenFeatures = 4;
        int numClasses = 2;

        Matrix A =
            new Matrix(
                numNodes,
                numNodes
            );

        A.data[0][1] = 1.0;
        A.data[1][0] = 1.0;

        A.data[0][2] = 1.0;
        A.data[2][0] = 1.0;

        A.data[1][3] = 1.0;
        A.data[3][1] = 1.0;

        A.data[2][3] = 1.0;
        A.data[3][2] = 1.0;

        A.print(
            "Original adjacency matrix A"
        );

        /*
         * ----------------------------------------------------------
         * STEP 2: NODE FEATURES
         *
         * Node 0 -> [1, 0]
         * Node 1 -> [1, 1]
         * Node 2 -> [0, 1]
         * Node 3 -> [0, 0]
         * ----------------------------------------------------------
         */

        Matrix X =
            new Matrix(
                numNodes,
                inputFeatures
            );

        X.data[0][0] = 1.0;
        X.data[0][1] = 0.0;

        X.data[1][0] = 1.0;
        X.data[1][1] = 1.0;

        X.data[2][0] = 0.0;
        X.data[2][1] = 1.0;

        X.data[3][0] = 0.0;
        X.data[3][1] = 0.0;

        X.print(
            "Node feature matrix X"
        );

        /*
         * ----------------------------------------------------------
         * STEP 3: LABELS
         * ----------------------------------------------------------
         */

        int[] labels = {
            0, 1, 0, 1
        };

        /*
         * ----------------------------------------------------------
         * STEP 4: NORMALIZE GRAPH
         * ----------------------------------------------------------
         */

        Matrix A_norm =
            normalizeAdjacency(A);

        A_norm.print(
            "Normalized adjacency matrix A_norm"
        );

        /*
         * ----------------------------------------------------------
         * STEP 5: CREATE MODEL
         * ----------------------------------------------------------
         */

        double learningRate = 0.05;

        GCNModel model =
            new GCNModel(
                A_norm,
                inputFeatures,
                hiddenFeatures,
                numClasses,
                learningRate,
                42
            );

        System.out.println(
            "\nModel configuration:"
        );

        System.out.println(
            "Input features : "
            + inputFeatures
        );

        System.out.println(
            "Hidden features: "
            + hiddenFeatures
        );

        System.out.println(
            "Output classes : "
            + numClasses
        );

        System.out.println(
            "Learning rate  : "
            + learningRate
        );

        /*
         * ----------------------------------------------------------
         * STEP 6: TRAIN
         * ----------------------------------------------------------
         */

        System.out.println(
            "\nTraining...\n"
        );

        int epochs = 1000;

        model.train(
            X,
            labels,
            epochs,
            100
        );

        /*
         * ----------------------------------------------------------
         * STEP 7: PREDICT
         * ----------------------------------------------------------
         */

        Matrix probabilities =
            model.predictProbabilities(X);

        probabilities.print(
            "Final class probabilities",
            6
        );

        int[] predictions =
            model.predict(X);

        System.out.println(
            "\nFinal predictions:"
        );

        for (int i = 0;
             i < numNodes;
             i++) {

            System.out.println(
                "Node " + i +
                " | True class: " +
                labels[i] +
                " | Predicted class: " +
                predictions[i]
            );
        }

        System.out.println(
            "\n===================================================="
        );

        System.out.println(
            "                  RUN COMPLETE"
        );

        System.out.println(
            "===================================================="
        );
    }
}
