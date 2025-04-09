#include<iostream>
#include<cstdlib>
using namespace std;

int main(){
    printf("Пять первых нечетных чисел: ");
    for(int i = 0;i<5;i++){
        printf("%d ", 2*i+1);
    }
    cout<<endl;
    return 0;
}