#pragma once

#include <Arduino.h>

using namespace std;

class TunnelRunner
{
private:
    enum Direction
    {
        Left = 0, // increase x
        Right = 1, // increase x
        Up = 2, // decrease y
        Down = 3 // increase y
    };

    const int TunnelSpeed = 1;
    const int RunnerSpeed = 2;
    const int CrashDelay = 30;
    const int ErrorDelay = 100;

    int _width;
    int _height;
    bool **_tunnelWalls;
    int _tunnelWidth;
    int _tunnelCooldown;

    Direction _runnerDirection;
    int _runnerLocation;
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
        uint32_t pathColor, uint32_t wallColor, uint32_t runnerColor,
        Direction runnerDirection,
        std::function<void(int, int, uint32_t)> drawPixel);

    void init();
    bool update(); // returns true if any pixel changed

private:
    bool advanceTunnel();
    bool moveRunner();
    void drawTunnel();
};

TunnelRunner::TunnelRunner(
    int width, int height,
    uint32_t pathColor, uint32_t wallColor, uint32_t runnerColor,
    Direction runnerDirection,
    std::function<void(int, int, uint32_t)> drawPixel)
{
    _width = width;
    _height = height;
    _pathColor = pathColor;
    _wallColor = wallColor;
    _runnerColor = runnerColor;
    _drawPixel = drawPixel;

    _tunnelWalls = new bool *[_height];
    for (int i = 0; i < _height; i++)
    {
        _tunnelWalls[i] = new bool[_width];
    }
}

void TunnelRunner::init()
{
    log_d("Initializing tunnel");

    // fill tunnel with walls (true)
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            _tunnelWalls[y][x] = true;
        }
    }

    // clear path (false)
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            _tunnelWalls[y][x] = false;
        }
    }

    // draw walls
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            _drawPixel(x, y, _tunnelWalls[y][x] ? _wallColor : _pathColor);
        }
    }
}

bool TunnelRunner::update()
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
    if (collision) {
        _resetDelay = CrashDelay;
        return true;
    }

    return update;
}

bool TunnelRunner::advanceTunnel()
{
    bool update = false;

    // shift tunnel down
    for (int y = _height - 1; y > 0; y--)
    {
        for (int x = 0; x < _width; x++)
        {
            _tunnelWalls[y][x] = _tunnelWalls[y - 1][x];
            _drawPixel(x, y, _tunnelWalls[y][x] ? _wallColor : _pathColor);
        }
    }

    // generate new row
    for (int x = 0; x < _width; x++)
    {
        _tunnelWalls[0][x] = random(2) == 0;
        _drawPixel(x, 0, _tunnelWalls[0][x] ? _wallColor : _pathColor);
    }

    return update;
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

    // Location prevRunnerLoc = _runnerLoc;
    // switch (_runnerDirection)
    // {
    // case Left:
    //     if (_runnerLoc.x > 0)
    //     {
    //         _runnerLoc.x--;
    //     }
    //     break;
    // case Right:
    //     if (_runnerLoc.x < _width - 1)
    //     {
    //         _runnerLoc.x++;
    //     }
    //     break;
    // case Up:
    //     if (_runnerLoc.y > 0)
    //     {
    //         _runnerLoc.y--;
    //     }
    //     break;
    // case Down:
    //     if (_runnerLoc.y < _height - 1)
    //     {
    //         _runnerLoc.y++;
    //     }
    //     break;
    // }

    // if (_tunnelWalls[_runnerLoc.y][_runnerLoc.x])
    // {
    //     _runnerLoc = prevRunnerLoc;
    //     _runnerCooldown = CrashDelay;
    //     _setStatus(RED);
    //     return false;
    // }

    // if (_runnerLoc == _exitLoc)
    // {
    //     _runnerLoc = prevRunnerLoc;
    //     _runnerCooldown = ErrorDelay;
    //     _setStatus(PURPLE);
    //     return false;
    // }

    // _drawPixel(prevRunnerLoc.x, prevRunnerLoc.y, _pathColor);
    // _drawPixel(_runnerLoc.x, _runnerLoc.y, _runnerColor);

    return true;
}

void TunnelRunner::drawTunnel()
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            _drawPixel(x, y, _tunnelWalls[y][x] ? _wallColor : _pathColor);
        }
    }
}