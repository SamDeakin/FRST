#pragma once

#include "SDL.h"
#include <queue>

#include "Interactions/InputEvent.hpp"


namespace FRST {
	namespace Interactions {
		class WindowSystem {
		public:
			WindowSystem(SDL_Window* window);
			~WindowSystem();

			// EventLoop functions
			/*
			 * Get all pending events by appending them to the provided queues
			 * Splits these events into game events and immediate events.
			 * Generally an immediate event is one that must be handled immediately,
			 * like a controller being disconnected or the window being resized.
			 *
			 * returns game events in gameEvents
			 * returns immediate events in immediateEvents
			 * returns the number of game events
			 */
			int getPendingEvents(
				std::queue<InputEvent*> &gameEvents,
				std::queue<InputEvent*> &immediateEvents);

			// These are not handled directly inside getPendingEvents to provide the caller
			// the chance to schedule their handling at a potentially later time.
			void handleWindowEvent(InputEvent* event);
		private:
			SDL_Window* window;
		};
	}
}
