#include "WindowSystem.hpp"

namespace FRST {
	WindowSystem::WindowSystem(SDL_Window* window) : window(window) {
	}

	WindowSystem::~WindowSystem() {
	}

	int WindowSystem::getPendingEvents(std::vector<FRST::IOEvent>& events) {
		int count = 0;

		SDL_Event temp_event;
		while (SDL_PollEvent(&temp_event)) {
			events.emplace_back(&temp_event);
			count++;
		}

		return count;
	}
}