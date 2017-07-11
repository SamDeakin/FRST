#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "IOEvent.hpp"


namespace FRST {
	class WindowSystem {
	public:
		WindowSystem(SDL_Window* window);
		~WindowSystem();

		// EventLoop functions
		/*
		 * Get all pending events by appending them to the provided vector
		 * Can be size 0
		 * Returns the number of events added
		 */
		int getPendingEvents(std::vector<FRST::IOEvent> &events);

	private:
		SDL_Window* window;
	};
}
