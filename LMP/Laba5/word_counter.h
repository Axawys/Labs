#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <string>
#include <fstream>

int count_short_words(const std::string& filename, int max_letters = 4);

#endif // WORD_COUNTER_H