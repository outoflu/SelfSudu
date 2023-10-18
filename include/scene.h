#ifndef __INCLUDE_SCENE__H
#define __INCLUDE_SCENE__H

#include <iostream>
#include <vector>
#include "common.h"
#include "block.h"
#include "command.h"

struct Scene {
    Scene(int index=3);
    virtual ~Scene();
    void generate();
    void show() const;
    void play();
    void save(const char* filename);
    void load(const char* filename);
    void eraseRandomGrids(const int count);
    bool isComplete();
    void setMode(KeyMode mode);
    bool setPointValue(const point_t&, const int);
    bool setCurValue(int,int&);
    point_t getCurPoint();
private:
    KeyMap* keyMap{};
    int _max_column;
    point_t _cur_point;
    Block _column_block[9];
    Block _row_block[9];
    Block _xy_block[3][3];
    point_value_t _map[81];
    std::vector<Command> _vCommand;
    void init();
    void setValue(const int);
    void setValue(const point_t&,const int);
    void printUnderline(int line_no=-1) const;
};


#endif