#ifndef DEFINITIONS_H__
#define DEFINITIONS_H__

#include "bankComponent.h"
#include "defenseComponent.h"
#include "motorComponent.h"
#include "offenseComponent.h"
#include "playerComponent.h"
#include "transformComponent.h"

#include "componentArray.h"

typedef ComponentArray<PlayerComponent, 4, 4> Players;
typedef ComponentArray<BankComponent, 4, 4> Banks;
typedef ComponentArray<MotorComponent, 36, 4> Motors;
typedef ComponentArray<OffenseComponent, 476, 8> Offenses;
typedef ComponentArray<TransformComponent, 476, 8> Transforms;

typedef Players::Iterator PlayerIterator;
typedef Banks::Iterator BankIterator;
typedef Motors::Iterator MotorIterator;
typedef Offenses::Iterator OffenseIterator;
typedef Transforms::Iterator TransformIterator;

#endif