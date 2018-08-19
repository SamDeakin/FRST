#pragma once

#include <SDL2/SDL.h>

#include <deque>
#include <unordered_map>

#include "IOEvent.hpp"


namespace FRST {
	namespace IO {
		typedef int GameControllerHandle; // TODO This is not the right place for this
		// TODO Should redo this entire strategy to include a wrapper that
		// discretizes each control method and forces the system to include a "player" index
		// for each control query.

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
			int getPendingEvents(std::deque<IOEvent> &events);

		private:
			void initControllers();

			SDL_Window* window;

			// Saved joystick id to controller mappings
			std::unordered_map<GameControllerHandle, SDL_GameController*> controllers;
		};
	}
}
