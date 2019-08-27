#include "Interactions/WindowSystem.hpp"

#include <iostream>


namespace FRST {
	namespace Interactions {
		WindowSystem::WindowSystem(SDL_Window* window) : window(window) {
		}

		WindowSystem::~WindowSystem() {
		}

		int WindowSystem::getPendingEvents(
			std::queue<InputEvent*> &gameEvents,
			std::queue<InputEvent*> &immediateEvents) {
			int count = 0;

			SDL_Event temp_event;
			while (SDL_PollEvent(&temp_event)) {
				InputEvent* newEvent = new InputEvent(&temp_event);
				if (newEvent->control.type == InputEvent::Type::UNSUPPORTED) {
					// Filter unsupported events
					delete newEvent;
					newEvent = nullptr;
				} else if (
					newEvent->isControllerModificationEvent() ||
					newEvent->isWindowEvent() ||
					newEvent->control.type == InputEvent::Type::QUIT) {
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

		void WindowSystem::handleWindowEvent(InputEvent* event) {
			// TODO not required for an MVP but should be written at some point.
			// I'll write this when not having it annoys me too much.
		}
	}
}