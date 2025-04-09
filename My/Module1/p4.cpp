//Вывод последовательности Фибоначи
#include<iostream>
using namespace std;

int input(){
    cout<<"Ввод: ";
    int n;
    for(;;){
        cin>>n;
        if (n>0 && n<1000)
            return n;
        cout<<"\nНедопустимый ввод, повтор: ";
    }
}

int main(){
    long long d1 = 0, d2 = 1;
    int n = input();
    cout<<"0, ";
    if(n<2)
        return 0;
    cout<<"1";
    if(n<3)
        return 0;
    for(int k = 0;k + 2 < n;k++){
        long long d3 = d2 + d1;
        d1 = d2;
        d2 = d3;
        cout<<", "<<d3;
    }
    cout<<endl;
    return 0;
}