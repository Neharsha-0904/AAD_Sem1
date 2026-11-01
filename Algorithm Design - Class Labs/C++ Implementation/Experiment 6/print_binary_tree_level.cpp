#include <iostream>
#include <queue>
#include <vector>
using namespace std;
struct Node { int val; Node *l,*r; Node(int x):val(x),left(nullptr),right(nullptr){} };
vector<int> printLevel(Node* root,int x) {
    vector<int> ans; if(!root||x<1)return ans;
    queue<pair<Node*,int>> q; q.push({root,1});
    while(!q.empty()) {
        auto [n,l]=q.front();q.pop();
        if(l==x) ans.push_back(n->val);
        else if(l<x) {
            if(n->left)q.push({n->left,l+1});
            if(n->right)q.push({n->right,l+1});
        }
    }
    return ans;
}
int main(){Node r(1),a(2),b(3);r.left=&a;r.right=&b;for(int x:printLevel(&r,2))cout<<x<<" ";}
