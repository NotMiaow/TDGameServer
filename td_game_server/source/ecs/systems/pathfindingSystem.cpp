#include "pathfindingSystem.h"

PathfindingSystem::PathfindingSystem(SharedQueue<Event*>& eventQueue, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
    m_eventQueue = &eventQueue;
    m_motors = &motors;
    m_transforms = &transforms;
}

void PathfindingSystem::Loop()
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CheckpointList<MotorComponent>::Iterator motorIt = m_motors->GetIterator(i, UNIT_GROUP_MOTORS);
        CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(i, UNIT_GROUP_TRANSFORMS);
        for(int j = 0;!motorIt.End(); j++, motorIt++, transformIt++)
            if(motorIt.Get()->behaviour == WaitingForPath)
                FindPath(i, j, *motorIt.Get(), *transformIt.Get());
    }
}

void PathfindingSystem::FindPath(const int& playerIndex, const int& motorPosition, MotorComponent& motor, const TransformComponent& transform)
{
    //Reset pathing fields
    if(!m_paths.empty()) m_shortestPath->path.clear();
    m_paths.clear();
    m_corners.clear();
    for(int i = 0; i < GRID_SIZE_Y; i++)
        for(int j = 0; j < GRID_SIZE_X; j++)
            m_pathinGrid[i][j] = false;

    //Generate pathing grid
    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(playerIndex, TOWER_TRANSFORMS);
    for(; !transformIt.End(); transformIt++)
        m_pathinGrid[(int)transformIt.Get()->position.y][(int)transformIt.Get()->position.x] = true;

    //Transform is in spawn
    if(transform.position.y < 0)
    {
        for(int i = 0; i < GRID_SIZE_X; i++)
            if(!m_pathinGrid[0][i])
            {
                std::vector<Vector2> path;
                path.push_back(Vector2(i, (int)transform.position.y));
                path.push_back(Vector2(i,0));
                Path newPath(path);
                m_paths.push_back(newPath);
            }
    }
    //Transform is in grid
    else
    {
        std::vector<Vector2> newPath;
        newPath.push_back(transformIt.Get()->position);
        m_paths.push_back(newPath);       
    }

    bool pathFound = false;
    //Find path
    while (!m_paths.empty() && !pathFound)
    {
        //Get shortest of current paths
        m_shortestPath = m_paths.begin();
        for(int i = 0; i < m_paths.size(); i++)
            if(m_paths.at(i).totalDistance < m_shortestPath->totalDistance)
                m_shortestPath = m_paths.begin() + i;

        if(SeekCornerVertically(m_shortestPath->path.back(), 1))
        {
            //END CONDITION - PATH FOUND
            if(m_shortestPath->path.back().y == GRID_SIZE_Y + DESPAWN_SIZE)
                pathFound = true;
        }
        else if(!SeekCornerVertically(m_shortestPath->path.back(), -1))
            m_paths.erase(m_shortestPath);
    }


    if(pathFound)
    {
        while(!motor.path.empty())
            motor.path.pop();

        motor.behaviour = Move;
        std::vector<Vector2>::iterator pathIt = m_shortestPath->path.begin();
        float distanceX = pathIt->x - transform.position.x;
        float distanceY = pathIt->y - transform.position.y;
        float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
        motor.normalizedTarget.x = distanceX / distance;
        motor.normalizedTarget.y = distanceY / distance;

        NewPathEvent* event = new NewPathEvent();
        for(;pathIt != m_shortestPath->path.end(); pathIt++)
        {
            motor.path.push(Vector2(pathIt->x, pathIt->y));
            event->path->push_back(Vector2(pathIt->x, pathIt->y));
        }

        event->playerPosition = playerIndex;
        event->motorPosition = motorPosition;
        m_eventQueue->push_back(event);
    }
    else motor.behaviour = Rage;
}

bool PathfindingSystem::SeekCornerVertically(const Vector2 cornerPosition, const int direction)
{
    bool pathPursued = false;
    Vector2 position = Vector2(cornerPosition.x, cornerPosition.y + direction);

    while(Pathable(position))
    {
        if(position.y < GRID_SIZE_Y - 1)
        {
            pathPursued = SeekCornerHorizontally(position, 1, pathPursued);
            pathPursued = SeekCornerHorizontally(position, -1, pathPursued);
        }
        else
        {
            if(pathPursued)
            {
                m_paths.push_back(*m_shortestPath);
                m_shortestPath = m_paths.end() - 1;
                m_shortestPath->path.pop_back();
                m_shortestPath->path.pop_back();
            }
            m_shortestPath->path.push_back(Vector2(cornerPosition.x, GRID_SIZE_Y + DESPAWN_SIZE));
            pathPursued = true;
        }

        position.y += direction;
    }
    return pathPursued;
}

bool PathfindingSystem::SeekCornerHorizontally(const Vector2 cornerPosition, const int direction, const bool createSubPath)
{
    bool pathPursued = createSubPath;
    Vector2 position = Vector2(cornerPosition.x, cornerPosition.y);
    Vector2 positionDown = Vector2(cornerPosition.x, cornerPosition.y + 1);
    Vector2 positionUp = Vector2(cornerPosition.x, cornerPosition.y - 1);
    bool wallUp = false, wallDown = false;
    bool previousWallUp = !Pathable(positionUp);
    bool previousWallDown = !Pathable(positionDown);

    position.x += direction;
    positionUp.x += direction;
    positionDown.x += direction;
    bool exit = false;
    while(Pathable(position) && !exit)
    {
        wallUp = !Pathable(positionUp);
        wallDown = !Pathable(positionDown);

        if(!wallUp && previousWallUp || !wallDown && previousWallDown)
        {
            const Vector2 shoulder = pathPursued ? *(m_shortestPath->path.end() - 3) : *(m_shortestPath->path.end() - 1);
            const int totalDistance = pathPursued ? m_shortestPath->previousDistance : m_shortestPath->totalDistance;
            const int verticalDistance = ToAbsoluteInt((int)(cornerPosition.y - shoulder.y));
            const int horizontalDistance = ToAbsoluteInt((int)(position.x - cornerPosition.x));

            if(AddCorner(cornerPosition, totalDistance + verticalDistance) && AddCorner(position, totalDistance + verticalDistance + horizontalDistance))
            {
                std::vector<Path>::iterator path = m_shortestPath;
                if(pathPursued)
                {
                    m_paths.push_back(*m_shortestPath);
                    m_shortestPath = m_paths.end() - 1;
                    m_shortestPath->path.pop_back();
                    m_shortestPath->path.pop_back();
                }
                m_shortestPath->path.push_back(cornerPosition);
                m_shortestPath->path.push_back(position);
                m_shortestPath->previousDistance = m_shortestPath->totalDistance;
                m_shortestPath->totalDistance = totalDistance + verticalDistance + horizontalDistance;
                m_shortestPath = path;
                pathPursued = true;
            }
            else exit = true;
        }

        previousWallUp = wallUp;
        previousWallDown = wallDown;
        position.x += direction;
        positionUp.x += direction;
        positionDown.x += direction;
    }
    return pathPursued;
}

bool PathfindingSystem::Pathable(const Vector2 position)
{
    if (position.x >= 0 && position.x < GRID_SIZE_X && position.y >= 0 && position.y < GRID_SIZE_Y)
        if(!m_pathinGrid[(int)position.y][(int)position.x])
            return true;
	return false;
}

bool PathfindingSystem::AddCorner(const Vector2 cornerPosition, const int totalDistanceToCorner)
{
    for(std::vector<Corner>::iterator cornerIt(m_corners.begin()); cornerIt != m_corners.end(); cornerIt++)
    {
        if(cornerIt->position.x == cornerPosition.x && cornerIt->position.y == cornerPosition.y)
        {
            if(totalDistanceToCorner <= cornerIt->minDistance)
            {
                cornerIt->minDistance = totalDistanceToCorner;
                return true;
            }
            return false;
        }
    }
    m_corners.push_back(Corner(cornerPosition, totalDistanceToCorner));
    return true;
}