/*
Routime.h

Abstract:
	file of the event things

*/

#ifndef _ROUTIME_H_ECS_
#define _ROUTIME_H_ECS_

#include "KernelDef.h"
#include <efi.h>

#define FUNCTION void

typedef unsigned long long LOOONG;

typedef _Bool BOOL;

typedef struct {
	BOOL trigged;
	BOOL NumTimerActive;
	LOOONG Timer;
} SysEvent;

SysEvent
StepEvent
(
	SysEvent EventE
)
{
	SysEvent Event;

	Event = EventE;
	if (Event.NumTimerActive == 1) {
		if (Event.Timer == 0) {
			Event.trigged == 1;
		}
		Event.Timer--;
		globalsystemtable->BootServices->Stall(1000000);
	}

	return Event;
}

FUNCTION
EventWait
(
	SysEvent Event
)
{
	SysEvent ev;

	ev = Event;

	while (!ev.trigged)
	{
		ev = StepEvent(ev);
	}
}
#endif // !_ROUTIME_H_ECS_
