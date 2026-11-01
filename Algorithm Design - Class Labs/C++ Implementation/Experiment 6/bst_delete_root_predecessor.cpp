#include <iostream>
#include <vector>
using namespace std;
struct Node { int key; Node *l,*r; Node(int x):key(x),left(nullptr),right(nullptr){} };
Node* deleteMax(Node* n,int& v) {
    if(!n->right){v=n->key;return n->left;}
    n->right=deleteMax(n->right,v);return n;
}
Node* deleteRootWithPredecessor(Node* root) {
    if(!root)return nullptr;
    if(!root->left)return root->right;
    if(!root->right)return root->left;
    int pred; root->left=deleteMax(root->left,pred);root->key=pred;return root;
}
void inorder(Node* n){if(!n)return;inorder(n->left);cout<<n->key<<" ";inorder(n->right);}
int main(){
    Node r(50),a(30),b(70),c(20),d(40);
    r.left=&a;r.right=&b;a.left=&c;a.right=&d;
    Node* root=deleteRootWithPredecessor(&r);inorder(root);cout<<"\n";
}
