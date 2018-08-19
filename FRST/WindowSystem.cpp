#include "WindowSystem.hpp"

#include <iostream>


namespace FRST {
	namespace IO {
		WindowSystem::WindowSystem(SDL_Window* window) : window(window) {
		}

		WindowSystem::~WindowSystem() {
		}

		int WindowSystem::getPendingEvents(
			std::queue<IOEvent*> &gameEvents,
			std::queue<IOEvent*> &immediateEvents) {
			int count = 0;

			SDL_Event temp_event;
			while (SDL_PollEvent(&temp_event)) {
				IOEvent* newEvent = new IOEvent(&temp_event);
				if (newEvent->control.type == IOEvent::Type::UNSUPPORTED) {
					// Filter unsupported events
					delete newEvent;
					newEvent = nullptr;
				} else if (
					newEvent->isControllerModificationEvent() ||
					newEvent->isWindowEvent()) {
					// This is an immediate event
					immediateEvents.push(newEvent);
					newEvent = nullptr;
				} else {
					// This is a game event
					gameEvents.push(newEvent);
					newEvent = nullptr;
					count++;
				}
			}

			return count;
		}

		void WindowSystem::handleWindowEvent(IOEvent* event) {
			// TODO not required for an MVP but should be written at some point.
			// I'll write this when not having it annoys me too much.
		}
	}
}