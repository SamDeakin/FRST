#include "Core.hpp"


namespace FRST {
	Core::Core(vk::Instance* instance, vk::SurfaceKHR* surface, SDL_Window* window)
		: m_ws(window)
		, m_controllerManager() {
	}

	Core::~Core() {
	}
	
	void Core::run() {
		m_running = true;
		int total_events = 0;

		// Queues for handling events
		std::queue<IO::IOEvent*> gameEvents;
		std::queue<IO::IOEvent*> immediateEvents;

		while (m_running) {

			int num_events = m_ws.getPendingEvents(gameEvents, immediateEvents);

			// Handle immediate events
			while (!immediateEvents.empty()) {
				IO::IOEvent* event = immediateEvents.front();
				immediateEvents.pop();

				if (event->isControllerModificationEvent()) {
					m_controllerManager.handleControllerEvent(event);
				} else if (event->isWindowEvent()) {
					m_ws.handleWindowEvent(event);
				} else if (event->control.type == IO::IOEvent::Type::QUIT) {
					// We quit here on this thread to hopefully exit well when the user asks us to.
					quit();
				}
				delete event;
			}

			// Handle game events
			while (m_running && !gameEvents.empty()) {
				IO::IOEvent* event = gameEvents.front();
				gameEvents.pop();

				// TODO Create an IOState object out of these.

				delete event;
			}

			total_events += num_events;

			SDL_Delay(10);
		}
	}

	void Core::quit() {
		m_running = false;
	}
} 