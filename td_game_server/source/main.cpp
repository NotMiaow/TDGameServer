#include <iostream>

#include "ecs.h"

int main()
{
    ECS ecs;
    ecs.Loop();
    std::cout << "temp message" << std::endl;
}
