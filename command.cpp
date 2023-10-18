#include "../include/scene.h"
#include "../include/command.h"

Command::Command(Scene *pOwner) : _pOwner(pOwner)
{}

Command::Command(Scene *pOwner, const point_t &point, int preValue, int curValue)
    : _pOwner(pOwner), _stPoint(point), _nPreValue(preValue), _nCurValue(curValue) {}

Command::Command(const Command &rhs)
: _pOwner(rhs._pOwner)
, _stPoint(rhs._stPoint)
, _nPreValue(rhs._nPreValue)
, _nCurValue(rhs._nCurValue)
{}


bool Command::execute(int nInputValue)
{
    if (!_pOwner)
        return false;

    _stPoint = _pOwner->getCurPoint();
    return _pOwner->setCurValue(nInputValue, _nPreValue);
}

void Command::undo()
{
    if (_pOwner)
    {
        _pOwner->setPointValue(_stPoint, _nPreValue);
    }

    return;
}