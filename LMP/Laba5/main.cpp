#include <iostream>
#include <string>
#include "word_counter.h"

int main() {
    const std::string filename = "infile.txt";
    
    int result = count_short_words(filename);
    
    if (result == -1) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return 1;
    }

    std::cout << "Количество слов с не более чем 4 буквами: " << result << std::endl;
    
    return 0;
}