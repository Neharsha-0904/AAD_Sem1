#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;
void dfs(int u,const vector<vector<int>>& g,vector<int>& state,vector<int>& order) {
    if(state[u]==1) throw runtime_error("Cycle detected");
    if(state[u]==2) return;
    state[u]=1; for(int v:g[u]) dfs(v,g,state,order);
    state[u]=2; order.push_back(u);
}
int main() {
    int n=6; vector<pair<int,int>> edges={{5,2},{5,0},{4,0},{4,1},{2,3},{3,1}};
    vector<vector<int>> g(n); for(auto [u,v]:edges) g[u].push_back(v);
    vector<int> state(n),order;
    try {
        for(int i=0;i<n;i++) if(!state[i]) dfs(i,g,state,order);
        reverse(order.begin(),order.end()); for(int x:order) cout<<x<<" ";
    } catch(...) { cout<<"Cycle detected"; }
    cout<<"\n";
}
