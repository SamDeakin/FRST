#include "IOEvent.hpp"

#include <iostream>


namespace FRST {
	IOEvent::IOEvent(SDL_Event* event) : x_motion(0), y_motion(0), x_relative(0), y_relative(0) {
		translateFromSDL(event);
	}

	void IOEvent::translateFromSDL(SDL_Event* event) {
		switch (event->type) {
		case SDL_CONTROLLERAXISMOTION:
			becomeControllerAxis(event->caxis);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			becomeControllerButton(event->cbutton);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			becomeKey(event->key);
			break;

		case SDL_MOUSEMOTION:
			becomeMouseMove(event->motion);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			becomeMouseButton(event->button);
			break;

		case SDL_MOUSEWHEEL:
			becomeMouseWheel(event->wheel);
			break;

		case SDL_QUIT:
			becomeQuit(event->quit);
			break;

		case SDL_WINDOWEVENT:
			becomeWindow(event->window);
			break;

		default:
			becomeUnsupported(*event);
			break;
		}
	}

	inline void IOEvent::becomeControllerAxis(SDL_ControllerAxisEvent& event) {

	}

	inline void IOEvent::becomeControllerButton(SDL_ControllerButtonEvent& event) {

	}

	inline void IOEvent::becomeKey(SDL_KeyboardEvent& event) {

	}

	inline void IOEvent::becomeMouseMove(SDL_MouseMotionEvent& event) {

	}

	inline void IOEvent::becomeMouseButton(SDL_MouseButtonEvent& event) {

	}

	inline void IOEvent::becomeMouseWheel(SDL_MouseWheelEvent& event) {

	}

	inline void IOEvent::becomeQuit(SDL_QuitEvent& event) {
		type = Type::QUIT;
#ifdef _DEBUG
		std::cout << "QUIT" << std::endl;
#endif
	}

	inline void IOEvent::becomeWindow(SDL_WindowEvent& event) {

	}

	inline void IOEvent::becomeUnsupported(SDL_Event& event) {
		type = Type::UNSUPPORTED;
#ifdef _DEBUG
		std::cout << "Unsupported: " << event.type << std::endl;
#endif
	}
}