#ifndef TOWER_COMPONENT_H__
#define TOWER_COMPONENT_H__

#include "gameEnums.h"
#include "position.h"

struct TowerComponent
{
	TowerType towerType;
	Position position;
};

#endif