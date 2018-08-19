#include "Core.hpp"
#include "IOState.hpp"

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

		IO::IOState* lastFrameState = new IO::IOState();

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

			std::vector<IO::IOEvent*> packagedEvents;
			packagedEvents.reserve(gameEvents.size());
			while (!gameEvents.empty()) {
				IO::IOEvent* event = gameEvents.front();
				gameEvents.pop();
				packagedEvents.push_back(event);
			}
			total_events += num_events;

			IO::IOState* frameState = new IO::IOState(*lastFrameState, packagedEvents);
			delete lastFrameState; // TODO This is a temporary clean up while we do nothing with the state right now.
			lastFrameState = frameState;

			SDL_Delay(10);
		}
	}

	void Core::quit() {
		m_running = false;
	}
} 