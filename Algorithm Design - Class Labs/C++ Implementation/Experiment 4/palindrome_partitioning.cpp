#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int minCuts(string s) {
    int n=s.size(); if(!n) return 0;
    vector<vector<bool>> p(n,vector<bool>(n,false)); vector<int> cut(n);
    for(int e=0;e<n;e++) {
        int best=e;
        for(int st=0;st<=e;st++)
            if(s[st]==s[e]&&(e-st<2||p[st+1][e-1])) {
                p[st][e]=true; best=min(best,st==0?0:cut[st-1]+1);
            }
        cut[e]=best;
    }
    return cut[n-1];
}
int main(){cout<<minCuts("aab")<<"\n";}
