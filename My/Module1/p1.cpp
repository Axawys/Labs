#include<iostream>
using namespace std;

int main(){
    int sum = 0;
    cout<<"Натуральные числа от 1 до 10:\n| ";
    for (int i=1;i<11;i++){
        cout<<i<<" | ";
        sum+=i;
    }
    cout<<endl<<"Сумма: "<<sum<<endl;
    return 0;
}