#include "FRST/Core.hpp"
#include <Interactions/InputState.hpp>

namespace FRST {
	Core::Core(vk::Instance* instance, vk::SurfaceKHR* surface, SDL_Window* window)
		: m_ws(window)
		, m_controllerManager() {
	}

	Core::~Core() noexcept {
	}

	void Core::run() {
		m_running = true;
		int total_events = 0;

		// Queues for handling events
		std::queue<Interactions::InputEvent*> gameEvents;
		std::queue<Interactions::InputEvent*> immediateEvents;

		Interactions::InputState* lastFrameState = new Interactions::InputState();

		while (m_running) {

			int num_events = m_ws.getPendingEvents(gameEvents, immediateEvents);

			// Handle immediate events
			while (!immediateEvents.empty()) {
				Interactions::InputEvent* event = immediateEvents.front();
				immediateEvents.pop();

				if (event->isControllerModificationEvent()) {
					m_controllerManager.handleControllerEvent(event);
				} else if (event->isWindowEvent()) {
					m_ws.handleWindowEvent(event);
				} else if (event->control.type == Interactions::InputEvent::Type::QUIT) {
					// We quit here on this thread to hopefully exit well when the user asks us to.
					quit();
				}
				delete event;
			}

			std::vector<Interactions::InputEvent*> packagedEvents;
			packagedEvents.reserve(gameEvents.size());
			while (!gameEvents.empty()) {
				Interactions::InputEvent* event = gameEvents.front();
				gameEvents.pop();
				packagedEvents.push_back(event);
			}
			total_events += num_events;

			Interactions::InputState* frameState = new Interactions::InputState(*lastFrameState, packagedEvents);
			delete lastFrameState; // TODO This is a temporary clean up while we do nothing with the state right now.
			lastFrameState = frameState;

			SDL_Delay(10);
		}
	}

	void Core::quit() {
		m_running = false;
	}
}