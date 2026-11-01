#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int main() {
    int n=6; vector<pair<int,int>> edges={{5,2},{5,0},{4,0},{4,1},{2,3},{3,1}};
    vector<vector<int>> g(n); vector<int> in(n);
    for(auto [u,v]:edges) g[u].push_back(v),in[v]++;
    priority_queue<int,vector<int>,greater<int>> pq;
    for(int i=0;i<n;i++) if(!in[i]) pq.push(i);
    vector<int> order;
    while(!pq.empty()) {
        int u=pq.top(); pq.pop(); order.push_back(u);
        for(int v:g[u]) if(--in[v]==0) pq.push(v);
    }
    if((int)order.size()!=n) cout<<"Cycle detected\n";
    else { for(int x:order) cout<<x<<" "; cout<<"\n"; }
}
