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

			// TODO These events should be queued as jobs instead of handled here
			while (!events.empty()) {
				IOEvent event = events.front();
				events.pop_front();

				if (event.type == IOEvent::Type::QUIT) {
					stillRunning = false;
				}
			}

			total_events += num_events;

			SDL_Delay(10);
		}
	}
} 