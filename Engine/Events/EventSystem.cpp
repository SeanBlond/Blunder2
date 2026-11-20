#include "EventSystem.h"

EventSystem* EventSystem::mspInstance = nullptr;

EventSystem* EventSystem::createInstance()
{
	if (mspInstance != nullptr)
		destroyInstance();

	mspInstance = new EventSystem();
	return mspInstance;
}
EventSystem* EventSystem::getInstance()
{
	return mspInstance;
}
void EventSystem::destroyInstance()
{
	//Check if there is something to delete
	if (mspInstance == nullptr)
	{
		std::cout << "Error, No Instance of Event System to Delete!" << std::endl;
		return;
	}

	//Delete
	delete mspInstance;
	mspInstance = nullptr;
}

void EventSystem::cleanup()
{
	mListeners.clear();
}

void EventSystem::addListener(EventType type, EventListener* listener)
{
	mListeners.insert({ type, listener });
}
void EventSystem::removeListener(EventType type, EventListener* listener)
{
	//Get all listeners of event
	std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::iterator> ret;
	ret = mListeners.equal_range(type);

	std::multimap<EventType, EventListener*>::iterator iter;
	for (iter = ret.first; iter != ret.second; ++iter)
	{
		if (iter->second == listener)
		{
			mListeners.erase(iter);
		}
	}
}
void EventSystem::removeListenerFromAllEvents(EventListener* pListener)
{
	std::multimap<EventType, EventListener*>::iterator iter;

	bool allTheWayThrough = false;

	while (!allTheWayThrough)
	{
		allTheWayThrough = true;
		for (iter = mListeners.begin(); iter != mListeners.end(); ++iter)
		{
			if (iter->second == pListener)
			{
				mListeners.erase(iter);
				allTheWayThrough = false; //didn't make it the whole way through
				break;//to prevent using invalidated iterator
			}
		}
	}
}

void EventSystem::fire(const Event& event)
{
	notifyListeners(event);
}

void EventSystem::notifyListeners(const Event& event)
{
	// Get all listeners of event
	std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::iterator> ret;
	ret = mListeners.equal_range(event.getType());

	// Tell all listeners the event
	std::multimap<EventType, EventListener*>::iterator iter;
	for (iter = ret.first; iter != ret.second; ++iter)
	{
		iter->second->handleEvent(event);
	}
}