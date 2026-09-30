#ifndef EVENT_SYSTEM
#pragma once

#include <iostream>
#include <map>
#include "Event.h"
#include "EventListener.h"

class Event;
class EventListener;
enum EventType;

class EventSystem
{
public:

	static EventSystem* createInstance();
	static EventSystem* getInstance();
	static void destroyInstance();

	void cleanup();

	void addListener(EventType type, EventListener* listener);
	void removeListener(EventType type, EventListener* listener);
	void removeListenerFromAllEvents(EventListener* pListener);

	void fire(const Event& event);

private:
	EventSystem() {}
	~EventSystem() { cleanup(); }

	static EventSystem* mspInstance;

	void notifyListeners(const Event& event);

	std::multimap<EventType, EventListener*> mListeners;
};

#endif // !EVENT_SYSTEM