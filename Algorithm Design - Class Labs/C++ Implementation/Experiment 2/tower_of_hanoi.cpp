#include <iostream>
using namespace std;
void hanoi(int n,char from,char aux,char to) {
    if(n==0) return;
    hanoi(n-1,from,to,aux);
    cout<<"Move disk "<<n<<" from "<<from<<" to "<<to<<"\n";
    hanoi(n-1,aux,from,to);
}
int main() {
    int n=3;
    cout<<"Minimum moves: "<<((1<<n)-1)<<"\n";
    hanoi(n,'A','B','C');
}
