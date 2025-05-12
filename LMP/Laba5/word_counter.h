#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <string>
#include <fstream>

// Функция для подсчета слов с не более чем max_letters буквами
int count_short_words(const std::string& filename, int max_letters = 4);

// Вспомогательная функция для проверки, является ли символ буквой
bool is_alpha_russian(unsigned char c);

#endif // WORD_COUNTER_H