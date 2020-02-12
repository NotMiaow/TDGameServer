#ifndef PATHFINDING_SYSTEM_H__
#define PATHFINDING_SYSTEM_H__

#include <algorithm>
#include <vector>
#include <math.h>
#include <tuple>

//Events
#include "event.h"
#include "shared_queue.h"

//Path
#include "vector2.h"
#include "corner.h"
#include "path.h"

#include "componentArray.h"
//Components
#include "motorComponent.h"
#include "transformComponent.h"

//Misc
#include "definitions.h"
#include "cst.h"

class PathfindingSystem{
public:
    PathfindingSystem() { }
    PathfindingSystem(SharedQueue<Event*>& eventQueue, Motors& motors, Transforms& transforms);
    ~PathfindingSystem() { }
    void Loop();
private:
    void FindPath(const int& playerIndex, const int& motorPosition, MotorComponent& motor, const TransformComponent& transform);
    bool SeekCornerVertically(const Vector2 cornerPosition, const int direction);
    bool SeekCornerHorizontally(const Vector2 cornerPosition, const int direction, const bool createSubPath);
    void Reset();
    void GetPathingGrid(const int& playerIndex);
    void SpawnInit(const TransformComponent& transform);
    void GridInit(const TransformComponent& transform);
    void SelectShortestPath();
    void SetPath(const int& motorId, MotorComponent& motor, const TransformComponent& transform);
    void SetRage(const int& motorId, MotorComponent& motor);
    bool Pathable(const Vector2 position);
    bool AddCorner(const Vector2 cornerPosition, const int totalDistanceToCorner);
private:
    SharedQueue<Event*>* m_eventQueue;
    Motors* m_motors;
    Transforms* m_transforms;

    bool m_pathinGrid[GRID_SIZE_Y][GRID_SIZE_X];
    std::vector<Path>::iterator m_shortestPath;
    std::vector<Path> m_paths;
    std::vector<Corner> m_corners;
};

#endif