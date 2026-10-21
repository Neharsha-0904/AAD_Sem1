import sys, unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import numpy as np
from src.gcn import normalize_adjacency, gcn_forward
from src.baby_kernel_style import aggregate_kernel_style

class TestGCN(unittest.TestCase):
    def setUp(self):
        self.a = np.array([[0,1,0],[1,0,1],[0,1,0]], float)
        self.x = np.array([[1,0],[0,1],[1,1]], float)
        self.w0 = np.array([[1,0.5],[-0.5,1]], float)
        self.w1 = np.array([[1,-1],[0.25,0.5]], float)
    def test_normalization(self):
        ah = normalize_adjacency(self.a)
        self.assertTrue(np.allclose(ah, ah.T))
        self.assertTrue(np.all(np.diag(ah) > 0))
    def test_kernel_style_matches_matrix(self):
        ah = normalize_adjacency(self.a)
        t = self.x @ self.w0
        self.assertTrue(np.allclose(aggregate_kernel_style(ah,t), np.maximum(ah@t,0)))
    def test_forward_outputs(self):
        r = gcn_forward(self.a,self.x,self.w0,self.w1)
        self.assertEqual(r["hidden"].shape,(3,2))
        self.assertEqual(r["probabilities"].shape,(3,2))
        self.assertTrue(np.allclose(r["probabilities"].sum(axis=1),1))
        self.assertEqual(r["predictions"].shape,(3,))
    def test_invalid_adjacency(self):
        with self.assertRaises(ValueError):
            normalize_adjacency(np.array([[1,2,3],[4,5,6]]))

if __name__ == "__main__":
    unittest.main()
