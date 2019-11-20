#ifndef EVENT_LANGUAGE_H__
#define	EVENT_LANGUAGE_H__

enum EventType
{
	EError,
	ECreateEvent, 
	EConnect,
	EDisconnect,
	EReadyUp,
	ESpawnUnitGroup,
	ENewPath,
	ERage,
	EBuildTower,
	ESellTower,
	ESendUnitGroup
};

enum GameErrorType
{
	GEWrongEventType,
	GEForbidden,
	GEWrongParemeterAmount,
	GEWrongParameterType,
	GEWrongSessionToken,
	GECouldNotCompleteRequestedAction,	//TEMPORARY ADD MORE COMPLEX FEEDBACK
	GEUnitGroupCapped,
	GEInsufficientFound,
	GECanNotBuildThere
};

//MISSING LOGGING ACTIONS

#endif