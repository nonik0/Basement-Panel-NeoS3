#pragma once

#include <Arduino.h>
#include <algorithm>
#include <functional>
#include <queue>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Coordinate
{
  int x;
  int y;
};

bool operator==(const Coordinate &lhs, const Coordinate &rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Coordinate &lhs, const Coordinate &rhs)
{
  return !(lhs == rhs);
}

namespace std
{
  template <>
  struct hash<Coordinate>
  {
    size_t operator()(const Coordinate &loc) const
    {
      return hash<int>()(loc.x) ^ hash<int>()(loc.y);
    }
  };
}

using Location = Coordinate;
using Direction = Coordinate;

const Direction Left = {-1, 0};
const Direction Right = {1, 0};
const Direction Up = {0, -1};
const Direction Down = {0, 1};
const Direction Directions[] = {Left, Right, Up, Down};

class MazeRunner
{
private:
  int _width;
  int _height;

  uint32_t _offColor;
  uint32_t _wallColor;

  uint32_t _runnerColor;
  Location _runnerLoc = {-1, -1};
  vector<Location> _runnerPath;
  int _runnerDistToExit = -1;

  uint32_t _sentryColor;
  Location _sentryLoc = {-1, -1};

  uint32_t _exitColor;
  Location _exitLoc = {-1, -1};

  bool **_mazeWalls;

  // function callback to draw pixels
  std::function<void(int, int, uint32_t)> _drawPixel;

public:
  MazeRunner(int width, int height, uint32_t offColor, uint32_t wallColor, uint32_t runnerColor, uint32_t sentryColor, uint32_t exitColor, std::function<void(int, int, uint32_t)> drawPixel);
  MazeRunner(int width, int height, uint32_t offColor, uint32_t wallColor, uint32_t runnerColor, uint32_t exitColor, std::function<void(int, int, uint32_t)> drawPixel)
      : MazeRunner(width, height, offColor, wallColor, runnerColor, offColor, exitColor, drawPixel) {}

  void init();
  void update();

private:
  void moveRunner();
  void moveSentry();
  void drawMaze();

  void generateMaze();
  void placeRunner();
  void placeSentry();
  void placeExit();

  vector<Location> findPathDfs(Location startLoc, Location endLoc, int maxDistToEnd = -1);
  vector<Location> findPathBfs(Location startLoc, Location endLoc);

  void shuffleDirections(Direction *list, int size);
  bool isWall(int x, int y);
  bool isWall(Location loc);
  bool isInMazeBounds(int x, int y);
  bool isInMazeBounds(Location loc);
  int getAdjacentWallAndBorderCount(int x, int y);
  int getAdjacentWallAndBorderCount(Location loc);
};

MazeRunner::MazeRunner(int width, int height, uint32_t offColor, uint32_t wallColor, uint32_t runnerColor, uint32_t sentryColor, uint32_t exitColor, std::function<void(int, int, uint32_t)> drawPixel)
{
  _width = width;
  _height = height;
  _offColor = offColor;
  _wallColor = wallColor;
  _runnerColor = runnerColor;
  _exitColor = exitColor;
  _drawPixel = drawPixel;

  _mazeWalls = new bool *[_height];
  for (int i = 0; i < _height; i++)
  {
    _mazeWalls[i] = new bool[_width];
  }
}

void MazeRunner::init()
{
  ("Initializing maze");

  generateMaze();
  placeRunner();
  placeExit();

  // // log maze in debug
  // log_d("*--------*");
  // for (int y = 0; y < _height; y++)
  // {
  //   String row = "|";
  //   for (int x = 0; x < _width; x++)
  //   {
  //     char c = isWall(x, y) ? '#' : ' ';
  //     c = (_runnerLoc.x == x && _runnerLoc.y == y) ? 'S' : c;
  //     c = (_exitLoc.x == x && _exitLoc.y == y) ? 'E' : c;
  //     row += c;
  //   }
  //   row += "|";
  //   log_d("%s", row);
  // }
  // log_d("*--------*");
}

void MazeRunner::update()
{
  moveRunner();
  drawMaze();
}

void MazeRunner::moveRunner()
{
  if (_runnerLoc == _exitLoc)
  {
    log_d("Runner reached exit");
    init();
    return;
  }
  else if (_runnerLoc == _sentryLoc)
  {
    log_d("Runner caught by sentry");
    init();
    return;
  }

  try
  {
    // vector<Location> path = findPathBfs(runnerLoc, exitLoc);

    if (_runnerPath.size() < 2)
    {
      _runnerPath = findPathDfs(_runnerLoc, _exitLoc, _runnerDistToExit);
    }

    if (_runnerPath.size() < 2)
    {
      log_d("No path found from (%d,%d) to (%d,%d)", _runnerLoc.x, _runnerLoc.y, _exitLoc.x, _exitLoc.y);
      return;
    }

    if (_runnerLoc != _runnerPath[0])
    {
      log_d("Runner is not at expected location (%d,%d)", _runnerPath[0].x, _runnerPath[0].y);
      return;
    }

    int stepDistance = abs(_runnerPath[1].x - _runnerPath[0].x) + abs(_runnerPath[1].y - _runnerPath[0].y);
    if (stepDistance != 1)
    {
      log_d("First step is not one step: (%d,%d) to (%d,%d)", _runnerPath[0].x, _runnerPath[0].y, _runnerPath[1].x, _runnerPath[1].y);
      return;
    }

    _runnerPath.erase(_runnerPath.begin());
    _runnerLoc = _runnerPath[0];
    _runnerDistToExit = _runnerPath.size() - 1; // -1 for included start

    log_d("Moved runner from (%d,%d) to (%d,%d) with dist %d", _runnerPath[0].x, _runnerPath[0].y, _runnerPath[1].x, _runnerPath[1].y, _runnerDistToExit);
  }
  catch (exception &e)
  {
    log_e("MazeRunnerMove Exception: %s", e.what());
  }
}

void MazeRunner::moveSentry()
{
}

void MazeRunner::drawMaze()
{
  for (int y = 0; y < _height; y++)
  {
    for (int x = 0; x < _width; x++)
    {
      _drawPixel(x, y, _mazeWalls[y][x] ? _wallColor : _offColor);
    }
  }

  _drawPixel(_exitLoc.x, _exitLoc.y, _exitColor);
  _drawPixel(_runnerLoc.x, _runnerLoc.y, _runnerColor);
  _drawPixel(_sentryLoc.x, _sentryLoc.y, _sentryColor);
}

void MazeRunner::generateMaze()
{
  log_d("Starting maze generation");

  // fill maze with walls (true)
  for (int y = 0; y < _height; y++)
  {
    for (int x = 0; x < _width; x++)
    {
      _mazeWalls[y][x] = true;
    }
  }

  // define starting point randomly if exit is not previously defined (ensures space for runner start loc)
  Location start = {-1, -1};
  if (_exitLoc.x == -1 || _exitLoc.y == -1)
  {
    int edge = random(4);
    int x = random(0, _width);
    int y = random(_height);
    start = {x, y};
  }
  else
  {
    start = _exitLoc;
  }
  _mazeWalls[start.y][start.x] = false;

  // create traversal stack with starting point
  stack<Location> path = stack<Location>();
  path.push(start);
  int maxCycles = 1000;

  while (!path.empty() && maxCycles-- > 0)
  {
    Location cur = path.top();

    // shuffle directions randomly
    Direction randSteps[4] = {Left, Right, Up, Down};
    shuffleDirections(randSteps, 4);

    // try to move in each direction, pop location if no path forward
    bool foundPath = false;
    for (int i = 0; i < 4; i++)
    {
      Location nextLoc = {cur.x + randSteps[i].x, cur.y + randSteps[i].y};
      if (isInMazeBounds(nextLoc) && isWall(nextLoc) && getAdjacentWallAndBorderCount(nextLoc) >= 3)
      {
        _mazeWalls[nextLoc.y][nextLoc.x] = false;
        path.push(nextLoc);
        foundPath = true;
        break;
      }
    }

    if (!foundPath)
    {
      path.pop();
    }
  }
  log_d("Maze generation complete");
}

void MazeRunner::placeRunner()
{
  _runnerLoc = {-1, -1};

  if (_exitLoc.x != -1 && _exitLoc.y != -1)
  {
    _runnerLoc = _exitLoc; // using last exit as start location
    return;
  }

  int attempts = 0;
  while (_runnerLoc.x == -1)
  {
    int x = random(_width);
    int y = random(_height);
    if (!isWall(x, y))
    {
      _runnerLoc = {x, y};
    }
    attempts++;
  }
  log_d("Placing runner at (%d,%d) after %d attempts", _runnerLoc.x, _runnerLoc.y, attempts);
}

void MazeRunner::placeSentry()
{
  _sentryLoc = {-1, -1};

  int attempts = 0;
  while (_sentryLoc.x == -1)
  {
    int x = random(_width);
    int y = random(_height);
    int distance = abs(x - _runnerLoc.x) + abs(y - _runnerLoc.y);
    int minDistance = max(0, (_width + _height) / 2 - (attempts / 10));
    if (!isWall(x, y) && distance > minDistance)
    {
      _sentryLoc = {x, y};
    }
    attempts++;
  }
  log_d("Placing sentry at (%d,%d) after %d attempts", _exitLoc.x, _exitLoc.y, attempts);
}

void MazeRunner::placeExit()
{
  _exitLoc = {-1, -1};

  int attempts = 0;
  while (_exitLoc.x == -1)
  {
    int x = random(_width);
    int y = random(_height);
    int distance = abs(x - _runnerLoc.x) + abs(y - _runnerLoc.y);
    int minDistance = max(0, (_width + _height) / 2 - (attempts / 10));
    if (!isWall(x, y) && distance > minDistance)
    {
      _exitLoc = {x, y};
    }
    attempts++;
  }
  log_d("Placing exit at (%d,%d) after %d attempts", _exitLoc.x, _exitLoc.y, attempts);
}

vector<Location> MazeRunner::findPathDfs(Location startLoc, Location endLoc, int maxDistToEnd)
{
  stack<pair<Location, int>> locsToVisit = stack<pair<Location, int>>();
  unordered_set<Location> locsVisited = unordered_set<Location>();
  stack<Location> curPath = stack<Location>();

  locsToVisit.push({startLoc, 0});

  while (!locsToVisit.empty())
  {
    pair<Location, int> curLocAndDist = locsToVisit.top();
    Location curLoc = curLocAndDist.first;
    int distFromStart = curLocAndDist.second;
    locsToVisit.pop();

    // if curPath size is greater than distance than we need to unwind path to current distance
    while (curPath.size() > distFromStart)
    {
      curPath.pop();
    }
    curPath.push(curLoc);

    // found end, return path in vector form
    if (curLoc == endLoc)
    {
      // log_v("Found path from (%d,%d) to (%d,%d)", startLoc.x, startLoc.y, curLoc.x, curLoc.y);

      std::vector<Location> path;
      while (!curPath.empty())
      {
        path.push_back(curPath.top());
        curPath.pop();
      }

      std::reverse(path.begin(), path.end());
      // reverse(path.begin(), path.end());
      return path;
    }

    locsVisited.insert(curLoc);

    // don't visit locations further than maxDistToEnd
    if (maxDistToEnd > 0 && (distFromStart + 1) > maxDistToEnd)
    {
      continue;
    }

    // look in different directions randomly in case of loops for variety of potential paths
    Direction randSteps[4] = {Left, Right, Up, Down};
    shuffleDirections(randSteps, 4);

    for (Direction step : randSteps)
    {
      Location nextLoc = {curLoc.x + step.x, curLoc.y + step.y};
      if (isInMazeBounds(nextLoc) && !isWall(nextLoc) && !locsVisited.count(nextLoc))
      {
        locsToVisit.push({nextLoc, distFromStart + 1});
      }
    }
  }

  return vector<Location>();
}

vector<Location> MazeRunner::findPathBfs(Location startLoc, Location endLoc)
{
  queue<Location> locsToVisit = queue<Location>();
  unordered_set<Location> locsVisited = unordered_set<Location>();
  unordered_map<Location, Location> visitedFrom = unordered_map<Location, Location>();

  locsToVisit.push(startLoc);
  visitedFrom[startLoc] = startLoc; // special case start location, visited from itself

  while (!locsToVisit.empty())
  {
    Location curLoc = locsToVisit.front();
    locsToVisit.pop();

    // found end, return path in vector form
    if (curLoc == endLoc)
    {
      log_d("Found path from (%d,%d) to (%d,%d)", startLoc.x, startLoc.y, curLoc.x, curLoc.y);

      vector<Location> path = vector<Location>();
      while (curLoc != startLoc)
      {
        path.push_back(curLoc);
        curLoc = visitedFrom[curLoc];
      }
      path.push_back(startLoc);

      std::reverse(path.begin(), path.end());
      // reverse(path.begin(), path.end());
      return path;
    }

    locsVisited.insert(curLoc);

    // look in different directions randomly in case of loops for variety of potential paths
    Direction randSteps[4] = {Left, Right, Up, Down};
    shuffleDirections(randSteps, 4);

    for (Direction step : randSteps)
    {
      Location nextLoc = {curLoc.x + step.x, curLoc.y + step.y};
      if (isInMazeBounds(nextLoc) && !isWall(nextLoc) && !locsVisited.count(nextLoc))
      {
        locsToVisit.push(nextLoc);
        visitedFrom[nextLoc] = curLoc;
      }
    }
  }

  return vector<Location>();
}

void MazeRunner::shuffleDirections(Direction *list, int size)
{
  for (int i = 0; i < size; i++)
  {
    int index = random(size);
    Direction temp = list[i];
    list[i] = list[index];
    list[index] = temp;
  }
}

bool MazeRunner::isWall(int x, int y)
{
  return _mazeWalls[y][x];
}

bool MazeRunner::isWall(Location loc)
{
  return isWall(loc.x, loc.y);
}

bool MazeRunner::isInMazeBounds(int x, int y)
{
  return x >= 0 && x < _width && y >= 0 && y < _height;
}

bool MazeRunner::isInMazeBounds(Location loc)
{
  return isInMazeBounds(loc.x, loc.y);
}

int MazeRunner::getAdjacentWallAndBorderCount(int x, int y)
{
  int count = 0;
  for (int i = 0; i < 4; i++)
  {
    int nx = x + Directions[i].x;
    int ny = y + Directions[i].y;
    if (nx < 0 || nx >= _width || ny < 0 || ny >= _height || _mazeWalls[ny][nx])
    {
      count++;
    }
  }
  return count;
}

int MazeRunner::getAdjacentWallAndBorderCount(Location loc)
{
  return getAdjacentWallAndBorderCount(loc.x, loc.y);
}
