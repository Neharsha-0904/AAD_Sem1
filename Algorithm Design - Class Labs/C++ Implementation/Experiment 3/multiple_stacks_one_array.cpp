#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;
class KStacks {
    vector<int> arr, top, nxt;
    int freeTop;
public:
    KStacks(int k,int cap):arr(cap),top(k,-1),nxt(cap),freeTop(cap?0:-1) {
        for(int i=0;i<cap-1;i++) nxt[i]=i+1;
        if(cap) nxt[cap-1]=-1;
    }
    void push(int s,int x) {
        if(s<0||s>=(int)top.size()) throw out_of_range("Invalid stack");
        if(freeTop==-1) throw overflow_error("Storage full");
        int i=freeTop; freeTop=nxt[i]; nxt[i]=top[s]; top[s]=i; arr[i]=x;
    }
    int pop(int s) {
        if(s<0||s>=(int)top.size()) throw out_of_range("Invalid stack");
        if(top[s]==-1) throw underflow_error("Stack empty");
        int i=top[s]; top[s]=nxt[i]; nxt[i]=freeTop; freeTop=i; return arr[i];
    }
    int peek(int s) const {
        if(top[s]==-1) throw underflow_error("Stack empty");
        return arr[top[s]];
    }
};
int main() {
    KStacks s(3,10); s.push(1,7); s.push(1,9); s.push(0,4);
    cout<<s.pop(1)<<" "<<s.peek(0)<<"\n";
}
