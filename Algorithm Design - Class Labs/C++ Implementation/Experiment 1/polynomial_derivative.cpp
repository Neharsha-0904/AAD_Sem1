#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
vector<pair<int,int>> derivative(const vector<pair<int,int>>& terms) {
    vector<pair<int,int>> ans;
    for(auto [c,e]:terms) if(e!=0 && c*e!=0) ans.push_back({c*e,e-1});
    sort(ans.begin(),ans.end(),[](auto a,auto b){return a.second>b.second;});
    return ans;
}
void show(const vector<pair<int,int>>& v) {
    if(v.empty()) { cout<<"0"; return; }
    for(int i=0;i<(int)v.size();i++) {
        auto [c,e]=v[i]; if(i) cout<<" + ";
        cout<<c; if(e>0) cout<<"x"; if(e>1) cout<<"^"<<e;
    }
    cout<<"\n";
}
int main() {
    vector<pair<int,int>> p={{3,2},{2,1},{5,0}};
    cout<<"Derivative: "; show(derivative(p));
}
