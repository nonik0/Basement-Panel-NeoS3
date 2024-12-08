#pragma once

#include <Arduino.h>

#include "coordinate.h"

class TunnelRunner
{
private:
    const int TunnelSpeed = 1;
    const int RunnerSpeed = 2;
    const int CrashDelay = 30;
    const int ErrorDelay = 100;
    const int TunnelMinWidth = 2;

    // constant
    int _width;
    int _height;
    int _tunnelVisibleLength;
    int _tunnelMaxWidth;
    Direction _tunnelDirection;
    bool _tunnelIsVertical;

    bool **_tunnelWalls;
    Location _tunnelLocation; // furthest from runner side, left or top empty space
    int _tunnelWidth;
    int _tunnelCooldown;
    bool _tunnelShrinking;

    Location _runnerLocation;
    int _runnerCooldown;
    int _resetDelay;

    uint32_t _pathColor;
    uint32_t _wallColor;
    uint32_t _runnerColor;

    // function callback to draw pixels
    std::function<void(int, int, uint32_t)> _drawPixel;
    // std::function<void(uint32_t)> _setStatus;

public:
    TunnelRunner(
        int width, int height,
        Direction tunnelDirection,
        uint32_t pathColor, uint32_t wallColor, uint32_t runnerColor,
        std::function<void(int, int, uint32_t)> drawPixel);

    void init();
    bool update(); // returns true if any pixel changed

private:
    bool advanceTunnel();
    bool moveRunner();
    void drawTunnel();

    bool isInBounds(int x, int y);
    bool isInBounds(Location loc);
    bool isWall(int x, int y);
    bool isWall(Location loc);
};

TunnelRunner::TunnelRunner(
    int width, int height,
    Direction tunnelDirection,
    uint32_t pathColor, uint32_t wallColor, uint32_t runnerColor,
    std::function<void(int, int, uint32_t)> drawPixel)
{
    _width = width;
    _height = height;
    _tunnelDirection = tunnelDirection;
    _tunnelIsVertical = tunnelDirection.x == 0;
    _tunnelMaxWidth = _tunnelIsVertical ? _width : _height;
    _tunnelVisibleLength = _tunnelIsVertical ? _height : _width;
    _pathColor = pathColor;
    _wallColor = wallColor;
    _runnerColor = runnerColor;
    _drawPixel = drawPixel;

    _tunnelWalls = new bool *[_width];
    for (int i = 0; i < _height; i++)
    {
        _tunnelWalls[i] = new bool[_height];
    }

    log_d("width: %d", _width);
    log_d("height: %d", _height);
    log_d("tunnelDirection: %d,%d", _tunnelDirection.x, _tunnelDirection.y);
    log_d("tunnelIsVertical: %d", _tunnelIsVertical);
    log_d("tunnelMaxWidth: %d", _tunnelMaxWidth);
    log_d("tunnelVisibleLength: %d", _tunnelVisibleLength);
}

void TunnelRunner::init()
{
    log_d("Initializing tunnel");

    _resetDelay = -1;
    _runnerCooldown = 0;
    _tunnelCooldown = 0;
    _tunnelShrinking = true;

    // fill tunnel with walls (true)
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            _tunnelWalls[x][y] = true;
        }
    }

    _tunnelWidth = _tunnelMaxWidth - 2;

    if (_tunnelDirection == Left)
    {
        _runnerLocation = {1, _height / 2}; // runner starts at middle-left
        _tunnelLocation = {0, 1};           // tunnel moves to left, so start at top-left
    }
    else if (_tunnelDirection == Right)
    {
        _runnerLocation = {_width - 2, _height / 2}; // runner starts at middle-right
        _tunnelLocation = {_width - 1, 1};           // tunnel moves to right, so start at top-right
    }
    else if (_tunnelDirection == Up)
    {
        _runnerLocation = {_width / 2, 1}; // runner starts at middle-top
        _tunnelLocation = {1, 0};          // tunnel moves up, so start at top-left
    }
    else if (_tunnelDirection == Down)
    {
        _runnerLocation = {_width / 2, _height - 2}; // runner starts at middle-bottom
        _tunnelLocation = {1, _height - 1};          // tunnel moves down, so start at bottom-left
    }

    // clear straight path to far side
    for (int i = 0; i < _tunnelVisibleLength; i++)
    {
        for (int j = 0; j < _tunnelWidth; j++)
        {
            if (_tunnelIsVertical)
            {
                _tunnelWalls[_tunnelLocation.x + j][_tunnelLocation.y] = false;
            }
            else
            {
                _tunnelWalls[_tunnelLocation.x][_tunnelLocation.y + j] = false;
            }
        }

        _tunnelLocation -= _tunnelDirection; // moving in opposite direction
    }

    _tunnelLocation += _tunnelDirection; // move back to edge
    log_d("Runner location: (%d,%d)", _runnerLocation.x, _runnerLocation.y);
    log_d("Tunnel location: (%d,%d)", _tunnelLocation.x, _tunnelLocation.y);
}

bool TunnelRunner::update()
{
    try
    {
        if (_resetDelay > 0)
        {
            _resetDelay--;
            if (_resetDelay <= 0)
            {
                init();
                drawTunnel();
                return true;
            }
            return false;
        }

        bool update = false;

        update |= moveRunner();
        update |= advanceTunnel();

        if (isWall(_runnerLocation))
        {
            log_d("Runner crashed");
            drawTunnel();
            _resetDelay = CrashDelay;
            return true;
        }

        if (update)
        {
            drawTunnel();
        }

        return update;
    }
    catch (const std::exception &e)
    {
        log_e("Error in tunnel runner update: %s", e.what());
        Serial.flush();
        delay(5000);
        _resetDelay = ErrorDelay;
        return false;
    }
}

bool TunnelRunner::advanceTunnel()
{
    if (_tunnelCooldown > 0)
    {
        _tunnelCooldown--;
        return false;
    }
    _tunnelCooldown = TunnelSpeed;

    // get location at left of vertical tunnel or top of horiz tunnel at near/runner side
    Location curLoc;
    if (_tunnelDirection == Left)
    {
        curLoc = {0, 0}; // tunnel moves to left, so start at top-left
    }
    else if (_tunnelDirection == Right)
    {
        curLoc = {_width - 1, 0}; // tunnel moves to right, so start at top-right
    }
    else if (_tunnelDirection == Up)
    {
        curLoc = {0, 0}; // tunnel moves up, so start at top-left
    }
    else if (_tunnelDirection == Down)
    {
        curLoc = {0, _height - 1}; // tunnel moves down, so start at bottom-left
    }

    // move all walls in direction of tunnel
    for (int i = 0; i < _tunnelVisibleLength - 1; i++)
    {
        for (int j = 0; j < _tunnelMaxWidth; j++)
        {
            // copy wall from opposite direction of tunnel movement
            if (_tunnelIsVertical)
            {
                _tunnelWalls[curLoc.x + j][curLoc.y] = _tunnelWalls[curLoc.x + j][curLoc.y - _tunnelDirection.y];
            }
            else
            {
                _tunnelWalls[curLoc.x][curLoc.y + j] = _tunnelWalls[curLoc.x - _tunnelDirection.x][curLoc.y + j];
            }
        }

        curLoc -= _tunnelDirection;
    }

    // randomly change tunnel width
    if (random() % 1000 == 0)
    {
        // expand and contract tunnel width
        _tunnelWidth += _tunnelShrinking ? -1 : 1;
        if (_tunnelWidth == TunnelMinWidth || _tunnelWidth == _tunnelMaxWidth)
        {
            _tunnelShrinking = !_tunnelShrinking;
        }
        log_i("Changed tunnel width to %d", _tunnelWidth);

        // when tunnel shrinks decide which direction it shifts randomly
        if (random() % 2 == 0)
        {
            _tunnelLocation = _tunnelIsVertical
                                  ? _tunnelLocation + Right
                                  : _tunnelLocation + Down;
        }
    }
    // randomly change tunnel location (shift center of tunnel)
    else if (random() % 10 >= 5)
    {
        Direction shiftDirection = _tunnelIsVertical
                                       ? (random() % 2 == 0 ? Left : Right)
                                       : (random() % 2 == 0 ? Up : Down);
        Location tunnelLocationOppositeSide = _tunnelIsVertical
                                                  ? _tunnelLocation + (_tunnelWidth - 1) * Right
                                                  : _tunnelLocation + (_tunnelWidth - 1) * Down;
        Location newLoc = _tunnelLocation + shiftDirection;
        Location newLocOpposite = tunnelLocationOppositeSide + shiftDirection;
        if (isInBounds(newLoc) && isInBounds(newLocOpposite))
        {
            _tunnelLocation = newLoc;
        }
    }

    // now generate new walls for tunnel at far end
    for (int i = 0; i < _tunnelMaxWidth; i++)
    {
        if (_tunnelIsVertical)
        {
            _tunnelWalls[i][_tunnelLocation.y] = i < _tunnelLocation.x || i >= _tunnelLocation.x + _tunnelWidth;
        }
        else
        {
            _tunnelWalls[_tunnelLocation.x][i] = i < _tunnelLocation.y || i >= _tunnelLocation.y + _tunnelWidth;
        }
    }

    _tunnelCooldown = TunnelSpeed;
    return true;
}

bool TunnelRunner::moveRunner()
{
    bool update = false;

    // move runner
    if (_runnerCooldown > 0)
    {
        _runnerCooldown--;
        return false;
    }

    // runner always tries to center self in tunnel

    // get average of tunnel width for next 3 sections ahead of runner
    int totalWidth = 0;
    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        Location lookAheadLoc = _runnerLocation - (i + 1) * _tunnelDirection;

        for (int j = 0; j < _tunnelMaxWidth; j++)
        {
            if (_tunnelIsVertical)
            {
                if (!isWall(j, lookAheadLoc.y))
                {
                    totalWidth += j;
                    count++;
                }
            }
            else
            {
                if (!isWall(lookAheadLoc.x, j))
                {
                    totalWidth += j;
                    count++;
                }
            }
        }
    }

    int lookAheadMiddleLoc = round((float)totalWidth / (float)count);
    int distToMiddle = lookAheadMiddleLoc - _runnerLocation.x;
    if (distToMiddle != 0)
    {
        Location newLoc = _runnerLocation + (_tunnelIsVertical
                                                 ? (distToMiddle > 0 ? Right : Left)
                                                 : (distToMiddle > 0 ? Down : Up));

        if (isInBounds(newLoc) && !isWall(newLoc))
        {
            // only reset cooldown if runner actually moved
            _runnerCooldown = RunnerSpeed;
            _runnerLocation = newLoc;
            update = true;
        }
    }

    return true;
}

void TunnelRunner::drawTunnel()
{
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            _drawPixel(x, y, _tunnelWalls[x][y] ? _wallColor : _pathColor);
        }
    }

    _drawPixel(_runnerLocation.x, _runnerLocation.y, _runnerColor);
}

bool TunnelRunner::isInBounds(int x, int y)
{
    return x >= 0 && x < _width && y >= 0 && y < _height;
}

bool TunnelRunner::isInBounds(Location loc)
{
    return isInBounds(loc.x, loc.y);
}

bool TunnelRunner::isWall(int x, int y)
{
    return _tunnelWalls[x][y];
}

bool TunnelRunner::isWall(Location loc)
{
    return isWall(loc.x, loc.y);
}