#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int knapsack(vector<int> w,vector<int> v,int cap) {
    vector<int> dp(cap+1,0);
    for(int i=0;i<(int)w.size();i++)
        for(int c=cap;c>=w[i];c--)
            dp[c]=max(dp[c],v[i]+dp[c-w[i]]);
    return dp[cap];
}
int main() {
    cout<<knapsack({1,3,4},{15,20,30},4)<<"\n";
}
