#ifndef PATHFINDING_SYSTEM_H__
#define PATHFINDING_SYSTEM_H__

#include <algorithm>
#include <vector>
#include <math.h>

//Events
#include "event.h"
#include "shared_queue.h"

//Path
#include "vector2.h"
#include "corner.h"
#include "path.h"

#include "checkpointList.h"
//Components
#include "motorComponent.h"
#include "transformComponent.h"

//Misc
#include "cst.h"

class PathfindingSystem{
public:
    PathfindingSystem() { }
    PathfindingSystem(SharedQueue<Event*>& eventQueue, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    ~PathfindingSystem() { }
    void Loop();
private:
    void FindPath(const int& playerIndex, const int& motorPosition, MotorComponent& motor, const TransformComponent& transform);
    bool SeekCornerVertically(const Vector2 cornerPosition, const int direction);
    bool SeekCornerHorizontally(const Vector2 cornerPosition, const int direction, const bool createSubPath);
    bool Pathable(const Vector2 position);
    bool AddCorner(const Vector2 cornerPosition, const int totalDistanceToCorner);
private:
    SharedQueue<Event*>* m_eventQueue;
    CheckpointList<MotorComponent>* m_motors;
    CheckpointList<TransformComponent>* m_transforms;

    bool m_pathinGrid[GRID_SIZE_Y][GRID_SIZE_X];
    std::vector<Path>::iterator m_shortestPath;
    std::vector<Path> m_paths;
    std::vector<Corner> m_corners;
};

#endif