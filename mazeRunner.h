#pragma once

#include <Arduino.h>
#include <algorithm>
#include <functional>
#include <queue>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#define ERROR_DELAY 10 * 1000

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
  const uint8_t RunnerFear = 5;
  const uint8_t RunnerSense = 2;
  const uint8_t RunnerSpeed = 12;
  const uint8_t SentrySense = 2;
  const uint8_t SentrySpeed = 20;
  const int SentryCatchDelay = 3000;

  int _width;
  int _height;

  int _mazeExtraWallsToRemove = 1;

  uint32_t _offColor;
  uint32_t _wallColor;

  Location _runnerLoc = {-1, -1};
  Location _runnerSentryKnownLoc = {-1, -1};
  stack<Location> _runnerPath;
  uint32_t _runnerColor;
  uint8_t _runnerCooldown = 0;
  int _runnerDistToExit = -1;
  int _runnerFleeDistance = 0;

  Location _sentryLoc = {-1, -1};
  stack<Location> _sentryPath;
  uint32_t _sentryColor;
  uint8_t _sentryCooldown = 0;

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
  bool update(); // returns true if any pixel changed

private:
  bool moveRunner();
  bool moveSentry();
  void drawMaze();

  void generateMaze();
  void placeRunner();
  void placeSentry();
  void placeExit();

  stack<Location> findPathDfs(Location startLoc, Location endLoc, int maxDistToEnd = -1) { return findPathDfs(startLoc, endLoc, {-1, -1}, maxDistToEnd); }
  stack<Location> findPathDfs(Location startLoc, Location endLoc, Location hazardLoc = {-1, -1}, int maxDistToEnd = -1);
  stack<Location> findPathBfs(Location startLoc, Location endLoc);

  void shuffleDirections(Direction *list, int size);
  bool isAdjacent(Location a, Location b) { return abs(a.x - b.x) + abs(a.y - b.y) == 1; }
  bool isKnownSentryLoc(int x, int y);
  bool isKnownSentryLoc(Location loc);
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
  _sentryColor = sentryColor;
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
  placeSentry();
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

bool MazeRunner::update()
{
  bool update = false;

  update |= moveRunner();
  update |= moveSentry();

  if (update)
  {
    drawMaze();
  }

  return update;
}

bool MazeRunner::moveRunner()
{
  if (_runnerCooldown > 0)
  {
    _runnerCooldown--;
    return false;
  }

  // TODO:
  // 1. sense sentry
  // 2. if sentry is sensed, update awareness
  // 3. check if path to exit is possible
  // 4. if path to exit is not possible, run away from sentry

  try
  {
    // sense
    stack<Location> sensedPathToSentry = findPathDfs(_runnerLoc, _sentryLoc, RunnerSense);
    if (sensedPathToSentry.size() > 0)
    {
      if (_runnerSentryKnownLoc == Location{-1, -1})
      {
        _runnerPath = findPathDfs(_runnerLoc, _exitLoc, _sentryLoc);
        log_v("Runner sensed sentry at (%d,%d)", _sentryLoc.x, _sentryLoc.y);
      }

      if (_runnerPath.size() > 0)
      {
        _runnerSentryKnownLoc = _sentryLoc;
      }

      _runnerSentryKnownLoc = _sentryLoc;
    }

    // move
    if (_runnerPath.size() > 0)
    {
      Location prevRunnerLoc = _runnerLoc;
      _runnerLoc = _runnerPath.top();
      _runnerPath.pop();
      _runnerCooldown = RunnerSpeed;
      // update = true;
      log_v("Moved runner from (%d,%d) to (%d,%d)", prevRunnerLoc.x, prevRunnerLoc.y, _runnerLoc.x, _runnerLoc.y);

      if (_runnerLoc == _exitLoc)
      {
        log_d("Runner reached exit");
        init();
        return true;
      }
    }
  }
  catch (exception &e)
  {
    log_e("MazeRunnerMove Exception: %s", e.what());
    delay(ERROR_DELAY);
    return false;
  }

  /*
  try
  {
    // vector<Location> path = findPathBfs(runnerLoc, exitLoc);

    if (_runnerFleeDistance > 0)
    {
      Direction awayX = _sentryLoc.x > _runnerLoc.x ? Left : Right;
      Direction awayY = _sentryLoc.y > _runnerLoc.y ? Up : Down;
      Direction towardsX = _sentryLoc.x > _runnerLoc.x ? Right : Left;
      Direction towardsY = _sentryLoc.y > _runnerLoc.y ? Down : Up;

      bool awayXFirst = random(2);
      bool towardsXFirst = random(2);
      Direction fleeDirections[4] = {
          awayXFirst ? awayX : awayY,
          awayXFirst ? awayY : awayX,
          towardsXFirst ? towardsX : towardsY,
          towardsXFirst ? towardsY : towardsX};

      for (Direction step : fleeDirections)
      {
        Location nextLoc = {step.x + _runnerLoc.x, step.y + _runnerLoc.y};
        if (isInMazeBounds(nextLoc) && !isWall(nextLoc) && nextLoc != _runnerSentryKnownLoc && nextLoc != _sentryLoc)
        {
          // check if can sense sentry
          vector<Location> pathToSentry = findPathDfs(_runnerLoc, _sentryLoc, RunnerSense);
          if (pathToSentry.size() > 0)
          {
            _runnerSentryKnownLoc = _sentryLoc;
            if (nextLoc == pathToSentry[0])
            {
              continue; // don't move into sentry's path
            }
          }
          else
          {
            _runnerFleeDistance--;
          }

          _runnerLoc = nextLoc;
          _runnerCooldown = RunnerSpeed;

          // feel safe, forget sentry location
          if (_runnerFleeDistance == 0)
          {
            _runnerPath = {};
            _runnerDistToExit = -1;
            _runnerSentryKnownLoc = {-1, -1};
            _runnerCooldown = RunnerSpeed * 2;
          }

          return true;
        }
      }

      return false;
    }

    if (_runnerPath.size() < 2)
    {
      log_d("Finding new path from (%d,%d) to (%d,%d)", _runnerLoc.x, _runnerLoc.y, _exitLoc.x, _exitLoc.y);
      _runnerPath = findPathDfs(_runnerLoc, _exitLoc, _runnerSentryKnownLoc, _runnerDistToExit);
    }

    if (_runnerPath.size() < 2)
    {
      if (_runnerSentryKnownLoc != Location{-1, -1}) // if path not found and sentry was seen, run away from sentry then try again
      {
        log_d("No path to goal found, fleeing from sentry at (%d,%d)", _sentryLoc.x, _sentryLoc.y);
        _runnerFleeDistance = RunnerFear;
        return false;
      }
      else
      {
        log_e("No path found from (%d,%d) to (%d,%d)", _runnerLoc.x, _runnerLoc.y, _exitLoc.x, _exitLoc.y);
        delay(ERROR_DELAY);
        _mazeExtraWallsToRemove++;
        init();
        return true;
      }
    }

    // cornered by sentry case, cannot run through sentry
    if (_runnerPath[1] == _sentryLoc) // if next step is sentry, run away from sentry
    {
      log_d("Cornered!?");
      _runnerFleeDistance = RunnerFear;
      return false;
    }

    // path is good, advance runner by one step
    Location prevRunnerLoc = _runnerLoc;
    _runnerPath.erase(_runnerPath.begin());
    _runnerLoc = _runnerPath[0];
    _runnerDistToExit = _runnerPath.size() - 1; // -1 for included start
    _runnerCooldown = RunnerSpeed;
    log_v("Moved runner from (%d,%d) to (%d,%d) with dist %d", prevRunnerLoc.x, prevRunnerLoc.y, _runnerLoc.x, _runnerLoc.y, _runnerDistToExit);

    if (_runnerLoc == _exitLoc)
    {
      log_d("Runner reached exit");
      init();
      return true;
    }

    // check if can sense sentry
    vector<Location> pathToSentry = findPathDfs(_runnerLoc, _sentryLoc, RunnerSense);
    if (pathToSentry.size() > 0)
    {
      log_v("Runner sensed sentry at (%d,%d)", _sentryLoc.x, _sentryLoc.y);
      _runnerSentryKnownLoc = _sentryLoc;
      _runnerPath = {};
      _runnerDistToExit = -1;
    }

    return true;
  }
  catch (exception &e)
  {
    log_e("MazeRunnerMove Exception: %s", e.what());
    delay(ERROR_DELAY);
    return false;
  }
  */
}

bool MazeRunner::moveSentry()
{
  if (_sentryColor == _offColor)
  {
    return false;
  }

  if (_sentryCooldown > 0)
  {
    _sentryCooldown--;
    return false;
  }

  // TODO: make sure this is decent location to check
  if (_sentryLoc == _runnerLoc)
  {
    log_d("Runner caught by sentry");
    delay(SentryCatchDelay);
    init();
  }

  // sense
  stack<Location> sensedPathToRunner = findPathDfs(_sentryLoc, _runnerLoc, SentrySense);
  if (sensedPathToRunner.size() > 0)
  {
    _sentryPath = sensedPathToRunner;

    // new detection, small "warm up" cooldown before moving
    if (_sentryPath.size() == 0) // path implies runner was seen recently
    {
      log_v("Sentry sensed runner at (%d,%d)", _runnerLoc.x, _runnerLoc.y);
      _sentryCooldown = SentrySpeed / 2;
      return false;
    }
  }

  // move
  if (_sentryPath.size() > 0)
  {
    Location prevSentryLoc = _sentryLoc;
    _sentryLoc = _sentryPath.top();
    _sentryPath.pop();
    _sentryCooldown = SentrySpeed;
    log_v("Moved sentry from (%d,%d) to (%d,%d)", prevSentryLoc.x, prevSentryLoc.y, _sentryLoc.x, _sentryLoc.y);
    return true;
  }

  return false;
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

  int wallsRemoved = 0;
  maxCycles = 1000;
  while (wallsRemoved < _mazeExtraWallsToRemove && maxCycles-- > 0)
  {
    int x = random(_width);
    int y = random(_height);
    if (isWall(x, y) && getAdjacentWallAndBorderCount(x, y) >= 2)
    {
      _mazeWalls[y][x] = false;
      wallsRemoved++;
    }
  }

  if (maxCycles <= 0)
  {
    log_e("Failed to remove extra walls");
  }

  log_d("Maze generation complete");
}

void MazeRunner::placeRunner()
{
  _runnerPath = {};
  _runnerDistToExit = -1;
  _runnerSentryKnownLoc = {-1, -1};
  _runnerFleeDistance = 0;
  _runnerCooldown = 0;

  if (_runnerLoc.x > 0 && _runnerLoc.y > 0)
  {
    if (_runnerLoc == _exitLoc)
    {
      log_d("Runner stays at prev exit loc at (%d,%d)", _runnerLoc.x, _runnerLoc.y);
      return;
    }
    else if (_runnerLoc == _sentryLoc)
    {
      log_d("Runner stays at prev sentry loc at (%d,%d)", _runnerLoc.x, _runnerLoc.y);
      return;
    }
    else
    {
      log_e("Runner unexpectedly not at exit or sentry loc: (%d,%d)", _runnerLoc.x, _runnerLoc.y);
    }
  }

  int attempts = 0;
  while (_runnerLoc.x < 0 || _runnerLoc.y < 0)
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
  if (_sentryColor == _offColor)
  {
    return;
  }

  _sentryLoc = {-1, -1};
  _sentryPath = {};
  _sentryCooldown = SentrySpeed;

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
  log_d("Placing sentry at (%d,%d) after %d attempts", _sentryLoc.x, _sentryLoc.y, attempts);
}

void MazeRunner::placeExit()
{
  _exitLoc = {-1, -1};

  int attempts = 0;
  while (_exitLoc.x == -1)
  {
    int x = random(_width);
    int y = random(_height);
    int runnerDistance = abs(x - _runnerLoc.x) + abs(y - _runnerLoc.y);
    int sentryDistance = abs(x - _sentryLoc.x) + abs(y - _sentryLoc.y);
    int minRunnerDistance = max(0, (_width + _height) / 2 - (attempts / 10));
    if (!isWall(x, y) && runnerDistance > minRunnerDistance && sentryDistance > 0)
    {
      _exitLoc = {x, y};
    }
    attempts++;
  }
  log_d("Placing exit at (%d,%d) after %d attempts", _exitLoc.x, _exitLoc.y, attempts);
}

stack<Location> MazeRunner::findPathDfs(Location startLoc, Location endLoc, Location hazardLoc, int maxDistToEnd)
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

    // found end, return reverse path stack and return
    if (curLoc == endLoc)
    {
      log_v("Found path from (%d,%d) to (%d,%d)", startLoc.x, startLoc.y, curLoc.x, curLoc.y);

      std::stack<Location> path;
      while (!curPath.empty())
      {
        path.push(curPath.top());
        curPath.pop();
      }

      // std::reverse(path.begin(), path.end());

      // remove start location from path
      // path.erase(path.begin());
      path.pop();

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
      if (isInMazeBounds(nextLoc) && !isWall(nextLoc) && (hazardLoc == Location{-1, -1} || (nextLoc != hazardLoc && !isAdjacent(nextLoc, hazardLoc))) && !locsVisited.count(nextLoc))
      {
        locsToVisit.push({nextLoc, distFromStart + 1});
      }
    }
  }

  return stack<Location>();
}

stack<Location> MazeRunner::findPathBfs(Location startLoc, Location endLoc)
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

      stack<Location> path = stack<Location>();
      while (curLoc != startLoc)
      {
        path.push(curLoc);
        curLoc = visitedFrom[curLoc];
      }
      path.push(startLoc);

      // std::reverse(path.begin(), path.end());
      //  reverse(path.begin(), path.end());

      path.pop();

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

  return stack<Location>();
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

bool MazeRunner::isKnownSentryLoc(int x, int y)
{
  return x == _runnerSentryKnownLoc.x && y == _runnerSentryKnownLoc.y;
}

bool MazeRunner::isKnownSentryLoc(Location loc)
{
  return isKnownSentryLoc(loc.x, loc.y);
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
