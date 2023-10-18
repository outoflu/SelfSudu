#include "../include/scene.h"
#include "../include/utility.inl"
#include <cmath>
#include <iostream>
#include <fstream>
#include <memory.h>
#include <vector>
#include <unordered_map>
using std::vector;

Scene::Scene(int index) : _max_column(pow(index, 2)), _cur_point({0, 0})
{
    init();
}

Scene::~Scene()
{
    if (keyMap)
    {
        delete keyMap;
        keyMap = nullptr;
    }
}

void Scene::show() const
{
    //system("clear");
    cls();
    printUnderline();
    for (int row = 0; row < _max_column; ++row)
    {
        Block block = _row_block[row];
        block.print();
        printUnderline(row);
    }
}

void Scene::setMode(KeyMode mode)
{
    if (mode == KeyMode::Normal)
    {
        keyMap = new Normal;
    }
    else
    {
        keyMap = new Vim;
    }
}

void Scene::printUnderline(int line_no) const
{
    for (int col = 0; col < 9; ++col)
    {
        std::cout << "\u254B"
                  << "\u2501" << ((_cur_point.y == line_no && _cur_point.x == col) ? "^" : "\u2501") << "\u2501";
    }
    std::cout << "\u254b" << std::endl;
}

void Scene::init()
{
    memset(_map, UNSELECTED, sizeof _map);

    // column_block 所有列
    for (int col = 0; col < _max_column; ++col)
    {
        Block column_block;

        for (int row = 0; row < _max_column; ++row)
        {
            column_block.push_back(_map + row * 9 + col);
        }
        _column_block[col] = column_block;
    }

    // row_block 所有行
    for (int row = 0; row < _max_column; ++row)
    {
        Block row_block;

        for (int col = 0; col < _max_column; ++col)
        {
            row_block.push_back(_map + row * 9 + col);
        }
        _row_block[row] = row_block;
    }

    // xy_block 所有九宫格, [行][列]
    for (int block_index = 0; block_index < _max_column; ++block_index)
    {
        Block xy_block;

        int xy_begin = block_index / 3 * 27 + block_index % 3 * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                xy_block.push_back(_map + xy_begin + i * 9 + j);
            }
        }
        _xy_block[block_index / 3][block_index % 3] = xy_block;
    }

    return;
}

bool Scene::setCurValue(const int nCurValue, int &nLastValue)
{
    auto point = _map[_cur_point.x + _cur_point.y * 9];
    if (Status::Erase == point.status)
    {
        nLastValue = point.value;
        setValue(nCurValue);
        return true;
    }
    else
    {
        return false;
    }
}

void Scene::setValue(const point_t &p, const int value)
{
    _map[p.x + p.y * 9].value = value;
}

void Scene::setValue(const int value)
{
    auto p = _cur_point;
    this->setValue(p, value);
}

void Scene::eraseRandomGrids(const int count)
{
    point_value_t pvt = {UNSELECTED, Status::Erase};
    vector<int> v(81);
    for (int i = 0; i < 81; ++i)
    {
        v[i] = i;
    }
    for (int i = 0; i < count; ++i)
    {
        int r = random(0, v.size() - 1);
        _map[v[r]] = pvt;
        v.erase(v.begin() + r);
    }
}

bool Scene::isComplete()
{
    for (size_t i = 0; i < 81; ++i)
    {
        if (_map[i].value == UNSELECTED)
            return false;
    }

    // 同时block里的数字还要符合规则
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (!_row_block[row].isValid() || 
                !_column_block[col].isValid() || 
                !_xy_block[row / 3][col / 3].isValid())
                return false;
        }
    }

    return true;
}

void Scene::save(const char *filename)
{
    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    for (int i = 0; i < 81; i++)
    {
        fs << _map[i].value << " " << static_cast<int>(_map[i].status) << std::endl;
    }

    fs << _cur_point.x << " " << _cur_point.y << std::endl;

    fs << _vCommand.size() << std::endl;
    for (auto &command : _vCommand)
    {
        point_t point = command._stPoint;
        fs << point.x << " " << point.y << ' '
           << command._nPreValue << ' '
           << command._nPreValue << std::endl;
    }

    fs.close();
}

void Scene::load(const char *filename)
{
    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    for (int i = 0; i < 81; i++)
    {
        int tmpStatus;
        fs >> _map[i].value >> tmpStatus;
        _map[i].status = static_cast<Status>(tmpStatus);
    }

    fs >> _cur_point.x >> _cur_point.y;

    size_t commandSize;
    fs >> commandSize;

    for (int i = 0; i < commandSize; i++)
    {
        point_t point;
        int preValue, curValue;
        fs >> point.x >> point.y >> curValue;
        _vCommand.emplace_back(this, point, preValue, curValue);
    }
}

void Scene::play()
{
    show();
    char key = '\0';
    while (true)
    {
        key=getch();
        if (key >= '0' && key <= '9')
        {
            Command oCommand(this);
            if (!oCommand.execute(key - '0'))
            {
                std::cout << "this number can't be modified" << std::endl;
            }
            else
            {
                _vCommand.push_back(std::move(oCommand));
                show();
                continue;
            }
        }
        if (keyMap->ESC == key)
        {
            message("quit game?[Y/N]");
            char input = 0;
            std::cin >> input;
            if (input == 'y' || input == 'Y')
            {
                message("do you want to save the progress ? [Y/N]");
                std::cin >> input;
                if (input == 'y' || input == 'Y')
                {
                    message("input path of progress file", false);
                    std::string file_path;
                    std::cin >> file_path;
                    save(file_path.c_str());
                }
                exit(0);
            }
            else
            {
                message("continue.");
            }
        }
        else if (key == keyMap->U)
        {
            if (_vCommand.empty())
            {
                message("no more action to undo.");
            }
            else
            {
                Command &oCommand = _vCommand.back();
                oCommand.undo();
                _vCommand.pop_back();
                show();
            }
        }
        else if (key == keyMap->LEFT)
        {
            _cur_point.x = (_cur_point.x - 1) < 0 ? 0 : _cur_point.x - 1;
            show();
        }
        else if (key == keyMap->RIGHT)
        {
            _cur_point.x = (_cur_point.x + 1) > 8 ? 8 : _cur_point.x + 1;
            show();
        }
        else if (key == keyMap->DOWN)
        {
            _cur_point.y = (_cur_point.y + 1) > 8 ? 8 : _cur_point.y + 1;
            show();
        }
        else if (key == keyMap->UP)
        {
            _cur_point.y = (_cur_point.y - 1) < 0 ? 0 : _cur_point.y - 1;
            show();
        }
        #if WIN32
        else if (key == keyMap->DIRECT)
        {
            key=getch();
            if (keyMap->_UP == key)
            {
                _cur_point.x = (_cur_point.y - 1) < 0 ? 0 : _cur_point.y - 1;
                show();
            }
            else if (keyMap->_LEFT == key)
            {
                _cur_point.x = (_cur_point.x - 1) < 0 ? 0 : _cur_point.x - 1;
                show();
            }
            else if (keyMap->_RIGHT == key)
            {
                _cur_point.x = (_cur_point.x + 1) > 8 ? 8 : _cur_point.x + 1;
                show();
            }
            else if (keyMap->_DOWN == key)
            {
                _cur_point.y = (_cur_point.y + 1) > 8 ? 8 : _cur_point.y + 1;
                show();
            }
        }
        #endif
        else if (key == keyMap->ENTER)
        {
            if (isComplete())
            {
                message("you win!");
                getchar();
                exit(0);
            }
            else
            {
                message("no completed.");
            }
        }
    }
}

void Scene::generate()
{
    static char map_pattern[10][10] = {
        "ighcabfde",
        "cabfdeigh",
        "fdeighcab",
        "ghiabcdef",
        "abcdefghi",
        "defghiabc",
        "higbcaefd",
        "bcaefdhig",
        "efdhigbca"};

    std::vector<char> v = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};

    // 产生字母到数字的随机映射
    std::unordered_map<char, int> hash_map;
    for (int i = 1; i <= 9; ++i)
    {
        int r = random(0, v.size() - 1);
        hash_map[v[r]] = i;
        v.erase(v.begin() + r);
    }

    // 填入场景
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            point_t point = {row, col};
            char key = map_pattern[row][col];
            setValue(point, hash_map[key]);
            //std::cout<<hash_map[key]<<" ";
        }
        //std::cout<<std::endl; 
    }

    assert(isComplete());

    return;
}

bool Scene::setPointValue(const point_t &stPoint, const int nValue)
{
    auto point = _map[stPoint.x + stPoint.y * 9];
    if (Status::Erase == point.status)
    {
        _cur_point = stPoint;
        setValue(nValue);
        return true;
    }
    return false;
}

point_t Scene::getCurPoint()
{
    return _cur_point;
}