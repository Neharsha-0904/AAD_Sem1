#include <iostream>
#include <queue>
using namespace std;
class QueueDS {
    queue<int> q;
public:
    void enqueue(int x) { q.push(x); }
    int dequeue() {
        if(q.empty()) throw runtime_error("Queue is empty");
        int x=q.front(); q.pop(); return x;
    }
    int peek() const {
        if(q.empty()) throw runtime_error("Queue is empty");
        return q.front();
    }
    void display() const {
        queue<int> copy=q;
        while(!copy.empty()) { cout<<copy.front()<<" "; copy.pop(); }
        cout<<"\n";
    }
};
int main() {
    QueueDS q; q.enqueue(10); q.enqueue(20); q.enqueue(30);
    q.display(); cout<<"Removed: "<<q.dequeue()<<"\n";
    cout<<"Front: "<<q.peek()<<"\n";
}
