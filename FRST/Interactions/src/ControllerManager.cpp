#include "Interactions/ControllerManager.hpp"


namespace FRST {
	namespace IO {
		ControllerManager::ControllerManager() {
			initControllers();
		}


		ControllerManager::~ControllerManager() {
			// Close the game controllers
			for (auto it = m_controllers.begin(); it != m_controllers.end(); it++) {
				SDL_GameController* controller = it->second;
				SDL_GameControllerClose(controller);
			}
		}

		void ControllerManager::initControllers() {
			int num_joysticks = SDL_NumJoysticks();

			// Add every joystick that is also a game controller
			for (int i = 0; i < num_joysticks; i++) {
				if (SDL_IsGameController(i)) {
					SDL_GameController* controller = SDL_GameControllerOpen(i);
					m_controllers[i] = controller;
				}
			}
		}

		void ControllerManager::handleControllerEvent(IOEvent* event) {
			// TODO Implement this at some point
			// Things will probably break if a controller disconnects ever
		}
	}
}