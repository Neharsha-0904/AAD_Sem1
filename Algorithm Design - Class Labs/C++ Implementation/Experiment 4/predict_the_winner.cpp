#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
bool predictWinner(vector<int> a) {
    int n=a.size(); if(!n) return true;
    vector<int> dp=a;
    for(int len=2;len<=n;len++)
        for(int l=0;l+len<=n;l++) {
            int r=l+len-1;
            dp[l]=max(a[l]-dp[l+1],a[r]-dp[l]);
        }
    return dp[0]>=0;
}
int main(){ cout<<(predictWinner({1,5,2})?"True":"False")<<"\n"; }
