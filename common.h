#ifndef __INCLUDE__COMMON__H
#define __INCLUDE__COMMON__H

static const unsigned int UNSELECTED=0;

enum class Difficulty : int {
    Easy=1,
    Normal,
    Hard
};

enum class Status :int {
    Init=0,
    Erase,
};

enum class KeyMode:int {
    Normal=1,
    VIM
};


struct KeyMap {
#if WIN32
    const char DIRECT=0xE0;
    const char _UP=0x48;
    const char _LEFT=0x4B;
    const char _DOWN=0x50;
    const char _RIGHT=0x4D;
#endif
    const char ESC =0x1B;
    const char U=0x75;
    char UP;
    char LEFT;
    char DOWN;
    char RIGHT;
    const char ENTER =0x0D;
};

struct Normal : KeyMap{
    Normal(){
        UP = 0x77;
        LEFT = 0x61;
        DOWN = 0x73;
        RIGHT = 0x64;
    }
};

struct Vim : KeyMap{
    Vim(){
        UP=0x6B;
        LEFT=0x68;
        DOWN=0x6A;
        RIGHT=0x6C;
    }
};

using point_t =struct point_t{
    int x;int y;
};

using point_value_t=struct point_value_t{
    int value;
    Status status;
};

auto EqualPoint=[](const point_t& p1,const point_t& p2){
    return (p1.x==p2.x&&p1.y==p2.y);
};

#endif