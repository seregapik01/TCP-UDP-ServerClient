#include "utils.h"

#include <vector>
#include <algorithm>

// очистка всех полей в структуре
void Numbers::ClearStorage()
{
    this->digits = "";
    this->digitsSum = "";
}

// парсит текст и заполняет на основе этого
// поля структуры numbers
void NumbersFromString(char text[],Numbers& numbers)
{   
    std::vector<int>sum_numbers;
    std::string str, s;
    int k = 0;
    str = text;
    str += ' ';
    for (int i = 0; i < str.size(); i++)
    {
        if (isdigit(str[i]))
            s += str[i];
        if (!(isdigit(str[i])))
        {
            if (s != ""){
                k++;
                sum_numbers.push_back(atoi(s.c_str())); 
            }
            s = "";
        }
    }
    int res_sum = 0;
    std::sort(begin(sum_numbers),end(sum_numbers));
    for(auto i :sum_numbers)
    {
        res_sum+=i;
    }
    numbers.digitsSum = std::to_string(res_sum);
    
    //for (const auto &piece : sum_numbers) numbers += piece;
    for(auto p:sum_numbers)
    {
        numbers.digits+=std::to_string(p);
        numbers.digits+=" ";
    }
}
