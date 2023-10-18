#ifndef __INCLUDE__BLOCK__H
#define __INCLUDE__BLOCK__H
#include "common.h"
#include "color.h"
#include <iostream>

struct Block
{
    static const int MAX_COUNT = 9;

    Block() : _count(0){};

    bool isValid() const
    {
        for (int i = 0; i < _count - 1; ++i)
        {
            for (int j = i + 1; j < _count; ++j)
            {
                if (UNSELECTED == _numbers[j]->value || UNSELECTED == _numbers[i]->value)
                {
                    continue;
                }
                if (_numbers[j]->value == _numbers[i]->value)
                {
                    return false;
                }
            }
        }
        return true;
    };

    bool isFull() const
    {
        for (int i = 0; i < _count; ++i)
        {
            point_value_t *p_point_value = _numbers[i];
            if (nullptr == p_point_value || UNSELECTED == p_point_value->value)
            {
                return false;
            }
        }
        return true;
    };

    void print() const
    {
        std::cout << "\u2503"
                  << " ";
        for (int i = 0; i < _count; ++i)
        {
            auto number = *(_numbers[i]);
            if (0 == number.value)
                std::cout << ' ' << " \u2503 ";
            else
            {
                if (number.status == Status::Erase)
                    std::cout << Modifier(FG_GREEN) << number.value << Modifier(RESET) << " \u2503 ";
                else
                    std::cout << number.value << " \u2503 ";
            }
        }
        std::cout << std::endl;
    };

    void push_back(point_value_t *point)
    {
        _numbers[_count++] = point;
    };
    int _count;
    point_value_t *_numbers[MAX_COUNT];
};

#endif