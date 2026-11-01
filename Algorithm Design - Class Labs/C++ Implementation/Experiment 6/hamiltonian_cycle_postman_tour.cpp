#include <iostream>
#include <vector>
#include <climits>
using namespace std;
void dfs(int u,int start,const vector<vector<int>>& g,vector<bool>& used,
         vector<int>& path,int cost,int& best,vector<int>& bestPath) {
    int n=g.size();
    if(path.size()==n) {
        if(g[u][start]>=0&&cost+g[u][start]<best) {
            best=cost+g[u][start];bestPath=path;bestPath.push_back(start);
        }return;
    }
    for(int v=0;v<n;v++) if(!used[v]&&g[u][v]>=0&&cost+g[u][v]<best) {
        used[v]=true;path.push_back(v);
        dfs(v,start,g,used,path,cost+g[u][v],best,bestPath);
        path.pop_back();used[v]=false;
    }
}
int main() {
    vector<vector<int>> g={{-1,10,15,20},{10,-1,35,25},{15,35,-1,30},{20,25,30,-1}};
    vector<bool> used(4);vector<int> path={0},bestPath;used[0]=true;int best=INT_MAX;
    dfs(0,0,g,used,path,0,best,bestPath);
    if(best==INT_MAX)cout<<"No Hamiltonian cycle\n";
    else {cout<<"Cost: "<<best<<" Path: ";for(int x:bestPath)cout<<x<<" ";cout<<"\n";}
}
