#ifndef __INCLUDE__COMMAND__H
#define __INCLUDE__COMMAND__H

#include <memory>
#include <iostream>
#include "common.h"
#include "scene.h"
struct Scene;
struct Command
{
    Command(Scene *pOwner);
    Command(Scene *pOwner, const point_t &point, int preValue, int curValue);
    Command(const Command &movement);

    void undo();

    /* data */

    bool execute(int inputValue);

    Scene *_pOwner;
    point_t _stPoint;
    int _nPreValue;
    int _nCurValue;
};

#endif