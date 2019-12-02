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

    //If the UnitGroup transform is in spawn
    if(transform.position.y < 0)
    {
        //Add y=0 positions which are not occupied by a Tower transform as starting position
        for(int i = 0; i < GRID_SIZE_X; i++)
            if(!m_pathinGrid[0][i])
            {
                std::vector<Vector2> path;
                path.push_back(Vector2(i,0));
                Path newPath(path);
                m_paths.push_back(newPath);
            }
    }
    //If the UnitGroup transform is on the grid
    else
    {   //Add it's position as the starting position
        std::vector<Vector2> newPath;
        newPath.push_back(transformIt.Get()->position);
        m_paths.push_back(newPath);       
    }

    bool pathFound = false;
    while (!m_paths.empty() && !pathFound)
    {
//        std::cout << std::endl << std::endl;
//        std::vector<Path>::iterator pathIt = m_paths.begin();
//        while(pathIt != m_paths.end())
//        {
//            std::vector<Vector2>::iterator position = pathIt->path.begin();
//            while(position != pathIt->path.end())
//            {
//                std::cout << "(" << position->y << ":" << position->x << ")";
//                position++;
//            }
//            std::cout << std::endl;
//            pathIt++;
//        }

        //Get the shortest of the current paths
        m_shortestPath = m_paths.begin();
        for(int i = 0; i < m_paths.size(); i++)
            if(m_paths.at(i).totalDistance < m_shortestPath->totalDistance)
                m_shortestPath = m_paths.begin() + i;

        //Always seek downwards first
        if(SeekCornerVertically(m_shortestPath->path.back(), 1))
        {
            //If a path is found from the shortest path
            if(m_shortestPath->path.back().y == GRID_SIZE_Y + DESPAWN_SIZE)
                pathFound = true;
        }
        //If downward seeking fails, seek upwards
        //If that fails too, remove the path
        else if(!SeekCornerVertically(m_shortestPath->path.back(), -1))
            m_paths.erase(m_shortestPath);

        //Then seek up
    }

    //Create a NewPathEvent
    NewPathEvent* event = new NewPathEvent();

    //Apply changes to the motor
    while(!motor.path.empty()) motor.path.pop();
    motor.behaviour = Move;
    std::vector<Vector2>::iterator pathIt = m_shortestPath->path.begin();
    float distanceX = pathIt->x - transform.position.x;
    float distanceY = pathIt->y - transform.position.y;
    float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    motor.normalizedTarget.x = distanceX / distance;
    motor.normalizedTarget.y = distanceY / distance;
    for(;pathIt != m_shortestPath->path.end(); pathIt++)
    {
        //Populate the event's path
        event->path->push_back(Vector2(pathIt->x, pathIt->y));
        //Populate the motor's path
        motor.path.push(Vector2(pathIt->x, pathIt->y));
    }
    std::cout << std::endl;

    //Create the new path event to be handled by the event manager
    event->playerPosition = playerIndex;
    event->motorPosition = motorPosition;
    m_eventQueue->push_back(event);
}

bool PathfindingSystem::SeekCornerVertically(const Vector2 cornerPosition, const int direction)
{
    bool pathPursued = false;
    Vector2 position = Vector2(cornerPosition.x, cornerPosition.y + direction);

    while(Pathable(position))
    {
//        std::cin.get();
//        std::cout << "vSeeking from corner: (" << position.y << ":" << position.x << ")" << std::endl;

        //If the current y position is GRID_SIZE - 1,
        //there is no need to seek horizontally because
        //a tower could not be placed outside of the grid.
        //This means that this path will be the shortest path.
        if(position.y < GRID_SIZE_Y - 1)
        {
            //Seek right first
            pathPursued = SeekCornerHorizontally(position, 1, pathPursued);
            //Then left
            pathPursued = SeekCornerHorizontally(position, -1, pathPursued);
        }
        //This path leads to the despawn
        else
        {
            //If the path was pursued
            if(pathPursued)
            {
                //Copy the pursued path
//                std::cout << "creating subPath***" << std::endl;
                m_paths.push_back(*m_shortestPath);
                //Set the copy as the current shortest path
                m_shortestPath = m_paths.end() - 1;
                //Remove the pursued path's found corners
                m_shortestPath->path.pop_back();
                m_shortestPath->path.pop_back();
            }
            //Add the position at despawn on the same column
            m_shortestPath->path.push_back(Vector2(cornerPosition.x, GRID_SIZE_Y + DESPAWN_SIZE));
            pathPursued = true;
        }

        //Move on the y axis
        position.y += direction;
    }
    return pathPursued;
}

bool PathfindingSystem::SeekCornerHorizontally(const Vector2 cornerPosition, const int direction, const bool createSubPath)
{
    bool pathPursued = createSubPath;
    //Sidewall statuses
    bool wallUp = false, wallDown = false;
    //Positions to consider
    Vector2 position = Vector2(cornerPosition.x, cornerPosition.y);
    Vector2 positionDown = Vector2(cornerPosition.x, cornerPosition.y + 1);
    Vector2 positionUp = Vector2(cornerPosition.x, cornerPosition.y - 1);
    //Set the previous wall statuses with the current side positions because
    //This x axis was already scanned by the last vertical seek
    bool previousWallUp = !Pathable(positionUp);
    bool previousWallDown = !Pathable(positionDown);
    //Since this x axis was already scanned by the last vertical seek
    //Increment positions's y before seeking
    position.x += direction;
    positionUp.x += direction;
    positionDown.x += direction;

    bool slowerPathing = false;
    while(Pathable(position) && !slowerPathing)
    {
//        std::cin.get();
//        std::cout << "hSeeking from corner: (" << position.y << ":" << position.x << ")" << std::endl;
        //Set current sidewall status
        wallUp = !Pathable(positionUp);
        wallDown = !Pathable(positionDown);

        //If there was previously a side wall on either side AND
        //there is currently no side wall on the same side
        if(!wallUp && previousWallUp || !wallDown && previousWallDown)
        {
//            std::cout << "pathPursued: " << pathPursued << std::endl;
            const Vector2 shoulder = pathPursued ? *(m_shortestPath->path.end() - 3) : *(m_shortestPath->path.end() - 1);
            const int totalDistance = pathPursued ? m_shortestPath->previousDistance : m_shortestPath->totalDistance;

            const int verticalDistance = ToAbsoluteInt((int)(cornerPosition.y - shoulder.y));
            const int horizontalDistance = ToAbsoluteInt((int)(position.x - cornerPosition.x));

//            std::cout << "totalDistance: " << totalDistance << std::endl;
//            std::cout << "verticalDistance: " << verticalDistance << std::endl;
//            std::cout << "horizontalDistance: " << horizontalDistance << std::endl;
//            std::cout <<  "shoulder: (" << shoulder.y << ":" << shoulder.x << ")" << std::endl;
//            std::cout <<  "elbow: (" << cornerPosition.y << ":" << cornerPosition.x << ")" << std::endl;
//            std::cout <<  "hand: (" << position.y << ":" << position.x << ")" << std::endl;
            if(AddCorner(cornerPosition, totalDistance + verticalDistance) &&
                AddCorner(position, totalDistance + verticalDistance + horizontalDistance))
            {
                std::vector<Path>::iterator path = m_shortestPath;
                if(pathPursued)
                {
//                    std::cout << "creating subPath***" << std::endl;
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
//                std::cout << "found elbow at: (" << cornerPosition.y << ":" << cornerPosition.x << ")(" <<  position.y << ":" << position.x << ")" << std::endl;
            }
            else slowerPathing = true;
        }

        //Remember current sidewall statuses for next loop
        previousWallUp = wallUp;
        previousWallDown = wallDown;
        //Move on the y axis
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
//            std::cout <<  totalDistanceToCorner << " vs " << cornerIt->minDistance;
            if(totalDistanceToCorner <= cornerIt->minDistance)
            {
                cornerIt->minDistance = totalDistanceToCorner;
                return true;
            }
            return false;
        }
    }
//    std::cout << "newCorner: (" << cornerPosition.y << ":" << cornerPosition.x << ") minDistance: " << totalDistanceToCorner << std::endl;
    m_corners.push_back(Corner(cornerPosition, totalDistanceToCorner));
    return true;
}