#include<iostream>
#include<cstdlib>
using namespace std;

int input(){
    cout<<"Ввод: ";
    int n;
    for(;;){
        cin>>n;
        if(n>0 && n<100)
            return n;
        cout<<"Неверный ввод, повторите: ";
    }
}

int main(){
    cout<<"Программа выводит числа, которые при делении на 4 дают остаток 3\nСколько чисел вывести?\n";
    int n = input();
    for(int k = 0;k<n;k++){
        printf("%d ", 4*k+3);
    }
    printf("\nПрограмма завершила работу\n");
    return 0;
}