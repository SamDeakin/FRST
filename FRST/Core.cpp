#include "Core.hpp"


namespace FRST {
	Core::Core(vk::Instance* instance, vk::SurfaceKHR* surface, SDL_Window* window) :
		ws(window) {
	}

	Core::~Core() {
	}
	
	void Core::run() {
		// Poll for user input.
		bool stillRunning = true;
		int total_events = 0;
		while (stillRunning) {

			int num_events = ws.getPendingEvents(events);

			for (int i = total_events; i < total_events + num_events; i++) {
				if (events[i].type == IOEvent::Type::QUIT) {
					stillRunning = false;
				}
			}

			total_events += num_events;

			SDL_Delay(10);
		}
	}
} 