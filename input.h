#ifndef __INCLUDE__INPUT__H
#define __INCLUDE__INPUT__H
#include <iostream>
#include <sstream>
#include <string>
#include "common.h"
#include "utility.inl"

int inputDifficulty()
{
    cls();
    std::string cmd;
    int need_erase_girds = 0;
    while (true)
    {
        std::cout << "设置难度：1简单 2普通 3困难" << std::endl;

        std::cin >> cmd;

        try
        {
            Difficulty difficulty = static_cast<Difficulty>(std::stoi(cmd));
            switch (difficulty)
            {
            case Difficulty::Easy:
                need_erase_girds = 20;
                break;
            case Difficulty::Normal:
                need_erase_girds = 35;
                break;
            case Difficulty::Hard:
                need_erase_girds = 50;
                break;
            default:
                break;
            }
        }
        catch (...)
        {
            need_erase_girds = 0;
        }
        if (need_erase_girds)
        {
            break;
        }
        std::cout << "输入错误" << std::endl;
    }
    return need_erase_girds;
}

KeyMode inputKeyMode()
{
    std::string mode;
    do {
        message("设置按键模式：1 正常  2 VIM");
        std::cin>> mode;
        try {
            KeyMode kmd=static_cast<KeyMode>(std::stoi(mode));
            return kmd;
        }
        catch(...){

        }
        message("输入错误");
    }
    while (true);
}

#endif