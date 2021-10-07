#include <string>

// числа и их сумма
struct Numbers
{
    std::string digits = "";
    std::string digitsSum = "";
    // очистка всех полей в структуре
    void ClearStorage();

};

// парсит текст и заполняет на основе этого
// поля структуры numbers
void NumbersFromString(char *text,Numbers& numbers);


