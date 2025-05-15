#include "word_counter.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <locale>

using namespace std;

int count_short_words(const std::string& filename, int max_letters) {
    ifstream infile(filename);
    if (!infile) {
        return -1;
    }

    // Устанавливаем локаль для поддержки UTF-8
    infile.imbue(locale("en_US.UTF-8"));
    
    int count = 0;
    string word;
    
    while (infile >> word) {
        // Считаем количество символов (не байт)
        int length = 0;
        for (char c : word) {
            if ((c & 0xC0) != 0x80) { // Подсчет только начальных байт UTF-8 символов
                length++;
            }
        }
        
        if (length <= max_letters) {
            count++;
        }
    }

    infile.close();
    return count;
}