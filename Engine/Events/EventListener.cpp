#include "EventListener.h"
#include "EventSystem.h"

EventListener::~EventListener()
{
	EventSystem::getInstance()->removeListenerFromAllEvents(this);
}