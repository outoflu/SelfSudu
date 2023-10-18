#ifndef __INCLUDE__UTILITY__INL
#define __INCLUDE__UTILITY__INL
#include <cstdlib>
#include <cassert>
#include <iostream>

void cls(){
#if WIN32
    system("cls");
#else
    system("clear");
#endif
}

#if __cplusplus > 201103L
#include <random>
std::default_random_engine dr_engine;
int random(int begin, int end)
{
    assert(begin <= end);
    std::uniform_int_distribution<int> distribution(begin, end);
    return distribution(dr_engine);
}

#else
#include <ctime>
int random(int begin, int end)
{
    assert(begin < end);
    srand(time(NULL));
    return (int)rand() % (end - begin + 1) + begin;
}
#endif

#ifdef _WIN32
#include <conio.h>
#else
    #ifdef __linux__
    #include <termio.h>
    #include <cstdio>
    #elif __APPLE__
    #include <termios.h>
    #endif
    inline char getch(void)
    {
        struct termios tmtemp, tm;
        char c;
        int fd = 0;
        if (tcgetattr(fd, &tm) != 0)
        { /*获取当前的终端属性设置，并保存到tm结构体中*/
            return -1;
        }
        tmtemp = tm;
        cfmakeraw(&tmtemp); /*将tetemp初始化为终端原始模式的属性设置*/
        if (tcsetattr(fd, TCSANOW, &tmtemp) != 0)
        { /*将终端设置为原始模式的设置*/
            return -1;
        }
        c = getchar();
        if (tcsetattr(fd, TCSANOW, &tm) != 0)
        { /*接收字符完毕后将终端设置回原来的属性*/
            return 0;
        }
        return c;
    }
#endif

void message(const char *message = "", bool changeLine = true)
{
    std::cout << message;
    if (changeLine)
        std::cout << std::endl;
}

#endif