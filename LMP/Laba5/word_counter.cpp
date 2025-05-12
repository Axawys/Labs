#include "word_counter.h"
#include <fstream>
#include <cctype>

bool is_alpha_russian(unsigned char c) {
    // Проверяем русские буквы в UTF-8 (кириллица)
    // или стандартные буквы ASCII
    return (c >= 0xC0 && c <= 0xFF) || std::isalpha(c);
}

int count_short_words(const std::string& filename, int max_letters) {
    std::ifstream infile(filename);
    if (!infile) {
        return -1; // Ошибка открытия файла
    }

    int count = 0;
    int current_word_length = 0;
    char c;

    while (infile.get(c)) {
        if (is_alpha_russian(c)) {
            current_word_length++;
        } else {
            if (current_word_length > 0 && current_word_length <= max_letters) {
                count++;
            }
            current_word_length = 0;
        }
    }

    // Проверяем последнее слово в файле
    if (current_word_length > 0 && current_word_length <= max_letters) {
        count++;
    }

    infile.close();
    return count;
}