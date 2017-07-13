#pragma once

#include <SDL2/SDL.h>

#include <deque>
#include <unordered_map>

#include "IOEvent.hpp"


namespace FRST {
	typedef int GameControllerHandle;

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
		int getPendingEvents(std::deque<FRST::IOEvent> &events);

	private:
		void initControllers();

		SDL_Window* window;

		// Saved joystick id to controller mappings
		std::unordered_map<GameControllerHandle, SDL_GameController*> controllers;
	};
}
