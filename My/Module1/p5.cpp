#include<iostream>
#include<cstdlib>
using namespace std;

int input(){
    int n;
    for(;;){
        cin>>n;
        if(n>0 && n<11)
            return n;
        cout<<"\nПопробуйте еще раз: ";
    }
}

int fact(int n){
    if(n <= 1)
        return 1;
    return n * fact(n-1);
}

int main(){
    cout<<"Функция считает биноминальные коэффициенты C из n по k, где n вводится с клавиатуры, а k с нуля до n.\n";
    cout<<"Введите n: ";
    int n = input();
    cout<<endl<<"k   = ";
    for(int i = 0;i<=n;i++)
        printf("%3d ", i);
    cout<<endl<<"c   = ";
    for(int i = 0;i<=n;i++){
        printf("%3d ", fact(n)/(fact(i)*fact(n-i)));
    }
    cout<<"\n\nПрограмма завершила работу!\n";
    return 0;
}