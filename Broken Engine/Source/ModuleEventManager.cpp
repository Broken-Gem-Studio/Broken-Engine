#include "ModuleEventManager.h"

#include "Optick/include/optick.h"

#include "mmgr/mmgr.h"

using namespace Broken;
ModuleEventManager::ModuleEventManager(bool start_enabled) {
	BROKEN_ASSERT(static_cast<int>(Event::EventType::invalid) == EVENT_TYPES - 1, "EVENT_TYPES macro needs to be updated!");
	name = "EventManager";
	head = 0;
	tail = 0;
}

ModuleEventManager::~ModuleEventManager() {
}

bool ModuleEventManager::Init(json& file) {


	return true;
}

bool ModuleEventManager::Start() {
	return true;
}

update_status ModuleEventManager::PreUpdate(float dt) {

	OPTICK_CATEGORY("Event Manager PreUpdate", Optick::Category::Network);
	while (head != tail) {
		// Process eventsW

		int EventType = static_cast<int>(events[head].type);

		for (std::vector<Function>::iterator it = listeners[EventType].listeners.begin(); it != listeners[EventType].listeners.end(); ++it) {
			// MYTODO: check if we can call the function first, delete listener if it is inaccessible
			(*it)(events[head]);
		}

		head = (head + 1) % MAX_EVENTS;
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleEventManager::CleanUp() {
	for (uint i = 0; i < EVENT_TYPES; ++i) {
		listeners[i].listeners.clear();
	}

	return true;
}

void ModuleEventManager::PushEvent(Event& new_event) {
	// --- Add given event to the end of the list ---
	events[tail] = new_event;
	tail = (tail + 1) % MAX_EVENTS;
}

void ModuleEventManager::AddListener(Event::EventType type, Function callback) {
	int EventType = static_cast<int>(type);
	bool found = false;

	for (std::vector<Function>::iterator it = listeners[EventType].listeners.begin(); it != listeners[EventType].listeners.end(); ++it) {
		if (*it == callback) {
			found = true;
			break;
		}
	}

	if (!found)
		listeners[EventType].listeners.push_back(callback);
}

void ModuleEventManager::RemoveListener(Event::EventType type, Function callback) {
	int EventType = static_cast<int>(type);

	for (std::vector<Function>::iterator it = listeners[EventType].listeners.begin(); it != listeners[EventType].listeners.end(); ++it) {
		if (*it == callback) {
			listeners[EventType].listeners.erase(it);
			break;
		}
	}
}
