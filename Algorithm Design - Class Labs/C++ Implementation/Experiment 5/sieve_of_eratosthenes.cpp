#include <iostream>
#include <vector>
using namespace std;
vector<int> sieve(int n) {
    if(n<2)return {};
    vector<bool> prime(n+1,true); prime[0]=prime[1]=false;
    for(int p=2;p*p<=n;p++) if(prime[p])
        for(int x=p*p;x<=n;x+=p) prime[x]=false;
    vector<int> ans; for(int i=2;i<=n;i++)if(prime[i])ans.push_back(i);
    return ans;
}
int main(){for(int p:sieve(20))cout<<p<<" ";cout<<"\n";}
