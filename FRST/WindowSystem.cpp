#include "WindowSystem.hpp"

#include <iostream>


namespace FRST {
	namespace IO {
		WindowSystem::WindowSystem(SDL_Window* window) : window(window) {
			initControllers();
		}

		WindowSystem::~WindowSystem() {
			// Close the game controllers
			for (auto it = controllers.begin(); it != controllers.end(); it++) {
				SDL_GameController* controller = it->second;
				SDL_GameControllerClose(controller);
			}
		}

		int WindowSystem::getPendingEvents(std::deque<IOEvent>& events) {
			int count = 0;

			SDL_Event temp_event;
			while (SDL_PollEvent(&temp_event)) {
				events.emplace_back(&temp_event);
				if (events.back().type == IOEvent::Type::UNSUPPORTED) {
					events.pop_back();
				}
				else {
					count++;
				}
			}

			return count;
		}

		void WindowSystem::initControllers() {
			int num_joysticks = SDL_NumJoysticks();

			// Add every joystick that is also a game controller
			for (int i = 0; i < num_joysticks; i++) {
				if (SDL_IsGameController(i)) {
					SDL_GameController* controller = SDL_GameControllerOpen(i);
					controllers[i] = controller;
				}
			}
		}
	}
}