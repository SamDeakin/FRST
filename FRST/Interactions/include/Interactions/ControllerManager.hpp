#pragma once

// #include <SDL2/SDL.h>
#include "SDL.h"

#include <unordered_map>

#include "Interactions/IOEvent.hpp"

namespace FRST {
	namespace IO {
		class ControllerManager {
			/*
			 * An opaque class to manage the connecting and disconnecting of controllers.
			 */
		public:
			ControllerManager();
			~ControllerManager();

			// Handle controller modification events like being disconnected/connected.
			void handleControllerEvent(IOEvent* event);
		private:
			void initControllers();

			// Saved joystick id to controller mappings
			std::unordered_map<IOEvent::Controller, SDL_GameController*> m_controllers;
		};
	}
}

