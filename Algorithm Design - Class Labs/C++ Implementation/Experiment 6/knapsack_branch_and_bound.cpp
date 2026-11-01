#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
struct Item{int w,v; double ratio;};
struct State{int level,w,v; double bound;};
struct Cmp{bool operator()(const State&a,const State&b)const{return a.bound<b.bound;}};
double bound(const State&s,const vector<Item>&it,int cap){
    if(s.w>=cap)return s.v;
    double b=s.v;int total=s.w,i=s.level;
    while(i<(int)it.size()&&total+it[i].w<=cap){total+=it[i].w;b+=it[i].v;i++;}
    if(i<(int)it.size())b+=(cap-total)*it[i].ratio;
    return b;
}
int solve(vector<int>w,vector<int>v,int cap){
    vector<Item> it;for(int i=0;i<w.size();i++)it.push_back({w[i],v[i],(double)v[i]/w[i]});
    sort(it.begin(),it.end(),[](auto&a,auto&b){return a.ratio>b.ratio;});
    priority_queue<State,vector<State>,Cmp> pq;State root{0,0,0,0};root.bound=bound(root,it,cap);pq.push(root);
    int best=0,n=it.size();
    while(!pq.empty()){
        State s=pq.top();pq.pop();if(s.bound<=best||s.level==n)continue;
        Item x=it[s.level];
        if(s.w+x.w<=cap){State a{s.level+1,s.w+x.w,s.v+x.v,0};best=max(best,a.v);a.bound=bound(a,it,cap);if(a.bound>best)pq.push(a);}
        State b{s.level+1,s.w,s.v,0};b.bound=bound(b,it,cap);if(b.bound>best)pq.push(b);
    }return best;
}
int main(){cout<<solve({2,3,5},{40,50,100},5)<<"\n";}
