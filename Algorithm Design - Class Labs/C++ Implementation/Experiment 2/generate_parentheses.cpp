#include <iostream>
#include <vector>
#include <string>
using namespace std;
void backtrack(int n,int open,int close,string s,vector<string>& out) {
    if((int)s.size()==2*n) { out.push_back(s); return; }
    if(open<n) backtrack(n,open+1,close,s+"(",out);
    if(close<open) backtrack(n,open,close+1,s+")",out);
}
int main() {
    int n=3; vector<string> ans;
    backtrack(n,0,0,"",ans);
    for(auto& s:ans) cout<<s<<"\n";
}
