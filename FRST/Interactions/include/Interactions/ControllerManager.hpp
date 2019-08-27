#pragma once

// #include <SDL2/SDL.h>
#include "SDL.h"

#include <unordered_map>

#include "Interactions/InputEvent.hpp"

namespace FRST {
	namespace Interactions {
		class ControllerManager {
			/*
			 * An opaque class to manage the connecting and disconnecting of controllers.
			 */
		public:
			ControllerManager();
			~ControllerManager();

			// Handle controller modification events like being disconnected/connected.
			void handleControllerEvent(InputEvent* event);
		private:
			void initControllers();

			// Saved joystick id to controller mappings
			std::unordered_map<InputEvent::Controller, SDL_GameController*> m_controllers;
		};
	}
}

