#include "IOEvent.hpp"

#include <iostream>


namespace FRST {
	IOEvent::IOEvent(SDL_Event* event) : x_motion(0), y_motion(0), x_relative(0), y_relative(0), control_method(-1) {
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

	void IOEvent::becomeControllerAxis(SDL_ControllerAxisEvent& event) {
		// TODO
#ifdef _DEBUG
		std::cout << "Controller Axis: " << event.axis << " " << event.value << std::endl;
#endif
	}

	void IOEvent::becomeControllerButton(SDL_ControllerButtonEvent& event) {
		// TODO
#ifdef _DEBUG
		std::cout << "Controller Button: " << event.button << std::endl;
#endif
	}

	void IOEvent::becomeKey(SDL_KeyboardEvent& event) {

	}

	void IOEvent::becomeMouseMove(SDL_MouseMotionEvent& event) {
		type = Type::MS_MOVE;
		x_motion = event.x;
		y_motion = event.y;
		x_relative = event.xrel;
		y_relative = event.yrel;
#ifdef _DEBUG
		std::cout << "Mouse Move: " << x_motion << " " << y_motion << " " << x_relative << " " << y_relative << std::endl;
#endif
	}

	static const IOEvent::Type mouseButtonToTypeMap[] = {
		IOEvent::Type::MS_B1,
		IOEvent::Type::MS_B2,
		IOEvent::Type::MS_B3,
		IOEvent::Type::MS_B4,
		IOEvent::Type::MS_B5,
		IOEvent::Type::MS_B6,
		IOEvent::Type::MS_B7,
		IOEvent::Type::MS_B8,
		IOEvent::Type::MS_B9,
		IOEvent::Type::MS_B10,
		IOEvent::Type::MS_B11,
		IOEvent::Type::MS_B12,
		IOEvent::Type::MS_B13,
		IOEvent::Type::MS_B14,
		IOEvent::Type::MS_B15,
		IOEvent::Type::MS_B16,
	};

	void IOEvent::becomeMouseButton(SDL_MouseButtonEvent& event) {
		type = mouseButtonToTypeMap[event.button - 1];
		x_motion = event.x;
		y_motion = event.y;
#ifdef _DEBUG
		std::cout << "Mouse Click: " << getTypeString(type) << " " << x_motion << " " << y_motion << std::endl;
#endif

	}

	void IOEvent::becomeMouseWheel(SDL_MouseWheelEvent& event) {
		type = Type::MS_WHEEL;
		x_motion = event.x;
		y_motion = event.y;
#ifdef _DEBUG
		std::cout << "Mouse Wheel: " << x_motion << " " << y_motion << std::endl;
#endif
	}

	void IOEvent::becomeQuit(SDL_QuitEvent& event) {
		type = Type::QUIT;
#ifdef _DEBUG
		std::cout << "QUIT" << std::endl;
#endif
	}

	void IOEvent::becomeWindow(SDL_WindowEvent& event) {

	}

	void IOEvent::becomeUnsupported(SDL_Event& event) {
		type = Type::UNSUPPORTED;
#ifdef _DEBUG
		std::cout << "Unsupported: " << event.type << std::endl;
#endif
	}

	const std::string& IOEvent::getTypeString(Type type) {
		auto it = TypeLabels.find(type);

		if (it == TypeLabels.end()) {
			return "[Undefined]";
		} else {
			return it->second;
		}
	}

	const std::unordered_map<IOEvent::Type, const std::string> IOEvent::TypeLabels = {
		{ IOEvent::Type::MS_B1, "Left Mouse Button" },
		{ IOEvent::Type::MS_B2, "Middle Mouse Button" },
		{ IOEvent::Type::MS_B3, "Right Mouse Button" },
		{ IOEvent::Type::MS_B4, "Mouse Button 4" },
		{ IOEvent::Type::MS_B5, "Mouse Button 5" },
		{ IOEvent::Type::MS_B6, "Mouse Button 6" },
		{ IOEvent::Type::MS_B7, "Mouse Button 7" },
		{ IOEvent::Type::MS_B8, "Mouse Button 8" },
		{ IOEvent::Type::MS_B9, "Mouse Button 9" },
		{ IOEvent::Type::MS_B10, "Mouse Button 10" },
		{ IOEvent::Type::MS_B11, "Mouse Button 11" },
		{ IOEvent::Type::MS_B12, "Mouse Button 12" },
		{ IOEvent::Type::MS_B13, "Mouse Button 13" },
		{ IOEvent::Type::MS_B14, "Mouse Button 14" },
		{ IOEvent::Type::MS_B15, "Mouse Button 15" },
		{ IOEvent::Type::MS_B16, "Mouse Button 16" },
		{ IOEvent::Type::MS_WHEEL, "Mouse Wheel" },
		{ IOEvent::Type::MS_MOVE, "Mouse Move" },

		// TODO Should we include two strings, one short and one long?
		{ IOEvent::Type::KB_F1, "F1" },
		{ IOEvent::Type::KB_F2, "F2 Key" },
		{ IOEvent::Type::KB_F3, "F3 Key" },
		{ IOEvent::Type::KB_F4, "F4 Key" },
		{ IOEvent::Type::KB_F5, "F5 Key" },
		{ IOEvent::Type::KB_F6, "F6 Key" },
		{ IOEvent::Type::KB_F7, "F7 Key" },
		{ IOEvent::Type::KB_F8, "F8 Key" },
		{ IOEvent::Type::KB_F9, "F9 Key" },
		{ IOEvent::Type::KB_F10, "F10 Key" },
		{ IOEvent::Type::KB_F11, "F11 Key" },
		{ IOEvent::Type::KB_F12, "F12 Key" },
		{ IOEvent::Type::KB_F13, "F13 Key" },
		{ IOEvent::Type::KB_F14, "F14 Key" },
		{ IOEvent::Type::KB_F15, "F15 Key" },
		{ IOEvent::Type::KB_F16, "F16 Key" },
		{ IOEvent::Type::KB_F17, "F17 Key" },
		{ IOEvent::Type::KB_F18, "F18 Key" },
		{ IOEvent::Type::KB_F19, "F19 Key" },
		{ IOEvent::Type::KB_F20, "F20 Key" },
		{ IOEvent::Type::KB_F21, "F21 Key" },
		{ IOEvent::Type::KB_F22, "F22 Key" },
		{ IOEvent::Type::KB_F23, "F23 Key" },
		{ IOEvent::Type::KB_F24, "F24 Key" },
		{ IOEvent::Type::KB_1, "1 Key" },
		{ IOEvent::Type::KB_2, "2 Key" },
		{ IOEvent::Type::KB_3, "3 Key" },
		{ IOEvent::Type::KB_4, "4 Key" },
		{ IOEvent::Type::KB_5, "5 Key" },
		{ IOEvent::Type::KB_6, "6 Key" },
		{ IOEvent::Type::KB_7, "7 Key" },
		{ IOEvent::Type::KB_8, "8 Key" },
		{ IOEvent::Type::KB_9, "9 Key" },
		{ IOEvent::Type::KB_0, "0 Key" },
		{ IOEvent::Type::KB_A, "A Key" },
		{ IOEvent::Type::KB_B, "B Key" },
		{ IOEvent::Type::KB_C, "C Key" },
		{ IOEvent::Type::KB_D, "D Key" },
		{ IOEvent::Type::KB_E, "E Key" },
		{ IOEvent::Type::KB_F, "F Key" },
		{ IOEvent::Type::KB_G, "G Key" },
		{ IOEvent::Type::KB_H, "H Key" },
		{ IOEvent::Type::KB_I, "I Key" },
		{ IOEvent::Type::KB_J, "J Key" },
		{ IOEvent::Type::KB_K, "K Key" },
		{ IOEvent::Type::KB_L, "L Key" },
		{ IOEvent::Type::KB_M, "M Key" },
		{ IOEvent::Type::KB_N, "N Key" },
		{ IOEvent::Type::KB_O, "O Key" },
		{ IOEvent::Type::KB_P, "P Key" },
		{ IOEvent::Type::KB_Q, "Q Key" },
		{ IOEvent::Type::KB_R, "R Key" },
		{ IOEvent::Type::KB_S, "S Key" },
		{ IOEvent::Type::KB_T, "T Key" },
		{ IOEvent::Type::KB_U, "U Key" },
		{ IOEvent::Type::KB_V, "V Key" },
		{ IOEvent::Type::KB_W, "W Key" },
		{ IOEvent::Type::KB_X, "X Key" },
		{ IOEvent::Type::KB_Y, "Y Key" },
		{ IOEvent::Type::KB_Z, "Z Key" },

		{ IOEvent::Type::KB_ALT_LEFT, "Left Alt Key" },
		{ IOEvent::Type::KB_ALT_RIGHT, "Right Alt Key" },
		{ IOEvent::Type::KB_ARROW_DOWN, "Down Arrow Key" },
		{ IOEvent::Type::KB_ARROW_LEFT, "Left Arrow Key" },
		{ IOEvent::Type::KB_ARROW_RIGHT, "Right Arrow Key" },
		{ IOEvent::Type::KB_ARROW_UP, "Up Arrow Key" },
		{ IOEvent::Type::KB_BACKSPACE, "Backspace Key" },
		{ IOEvent::Type::KB_BACKTICK, "` Key" },
		{ IOEvent::Type::KB_BRACE_LEFT, "[ Key" },
		{ IOEvent::Type::KB_BRACE_RIGHT, "] Key" },
		{ IOEvent::Type::KB_CAPSLOCK, "Caps Lock Key" },
		{ IOEvent::Type::KB_COMMA, ", Key" },
		{ IOEvent::Type::KB_CONTEXT_MENU, "That Other Weird key" },
		{ IOEvent::Type::KB_CONTROL_LEFT, "Left Control Key" },
		{ IOEvent::Type::KB_CONTROL_RIGHT, "Right Control Key" },
		{ IOEvent::Type::KB_EQUALS, "= Key" },
		{ IOEvent::Type::KB_ENTER, "Enter Key" },
		{ IOEvent::Type::KB_ESC, "Escape Key" },
		{ IOEvent::Type::KB_MINUS, "- Key" },
		{ IOEvent::Type::KB_PERIOD, ". Key" },
		{ IOEvent::Type::KB_SHIFT_LEFT, "Left Shift Key" },
		{ IOEvent::Type::KB_SHIFT_RIGHT, "Right Shift Key" },
		{ IOEvent::Type::KB_SLASH_LEFT, "\\ Key" },
		{ IOEvent::Type::KB_SLASH_RIGHT, "/ Key" },
		{ IOEvent::Type::KB_SPACE, "Space Key" },
		{ IOEvent::Type::KB_SUPER_LEFT, "Left Super Key" },
		{ IOEvent::Type::KB_SUPER_RIGHT, "Right Super Key" },
		{ IOEvent::Type::KB_TAB, "Tab Key" },

		{ IOEvent::Type::CTRL_B_A, "A Button" },
		{ IOEvent::Type::CTRL_B_B, "B Button" },
		{ IOEvent::Type::CTRL_B_X, "X Button" },
		{ IOEvent::Type::CTRL_B_Y, "Y Button" },
		{ IOEvent::Type::CTRL_B_BACK, "Controller Back Button" },
		{ IOEvent::Type::CTRL_B_GUIDE, "Controller Guide Button" },
		{ IOEvent::Type::CTRL_B_START, "Start Button" },
		{ IOEvent::Type::CTRL_B_STICK_LEFT, "Left Stick" },
		{ IOEvent::Type::CTRL_B_STICK_RIGHT, "Right Stick" },
		{ IOEvent::Type::CTRL_B_SHOULDER_LEFT, "Left Shoulder Button" },
		{ IOEvent::Type::CTRL_B_SHOULDER_RIGHT, "Right Shoulder Button" },
		{ IOEvent::Type::CTRL_B_DPAD_DOWN, "DPAD Down" },
		{ IOEvent::Type::CTRL_B_DPAD_LEFT, "DPAD Left" },
		{ IOEvent::Type::CTRL_B_DPAD_RIGHT, "DPAD Right" },
		{ IOEvent::Type::CTRL_B_DPAD_UP, "DPAD Up" },
		{ IOEvent::Type::CTRL_AXIS_LEFT_X, "Left Stick X Axis" },
		{ IOEvent::Type::CTRL_AXIS_LEFT_Y, "Left Stick Y Axis" },
		{ IOEvent::Type::CTRL_AXIS_RIGHT_X, "Right Stick X Axis" },
		{ IOEvent::Type::CTRL_AXIS_RIGHT_Y, "Right Stick Y Axis" },
		{ IOEvent::Type::CTRL_AXIS_TRIGGER_LEFT, "Left Trigger Axis" },
		{ IOEvent::Type::CTRL_AXIS_TRIGGER_RIGHT, "Right Trigger Axis" },
		{ IOEvent::Type::CTRL_ADDED, "Controller Added" },
		{ IOEvent::Type::CTRL_REMOVED, "Controller Removed" },
		{ IOEvent::Type::CTRL_REMAPPED, "Controller Remapped" },

		{ IOEvent::Type::QUIT, "Quit Application" },
		{ IOEvent::Type::MINIMIZED, "Window Minimized" },
		{ IOEvent::Type::MAXIMIZED, "Window Maximized" },
		{ IOEvent::Type::RESTORED, "Window Restored" },
		{ IOEvent::Type::MOUSE_FOCUS_GAINED, "Mouse Focus Gained" },
		{ IOEvent::Type::MOUSE_FOCUS_LOST, "Mouse Focus Lost" },
		{ IOEvent::Type::KEYBOARD_FOCUS_GAINED, "Keyboard Focus Gained" },
		{ IOEvent::Type::KEYBOARD_FOCUS_LOST, "Keyboard Focus Lost" },

		{ IOEvent::Type::MOVE, "Window Moved" },
		{ IOEvent::Type::CLOSE, "Window Closed" },
		{ IOEvent::Type::RESIZE, "Window Resized" },
		{ IOEvent::Type::MINIMIZE, "Window Minimized" },
		{ IOEvent::Type::FULLSCREEN, "Window Fullscreened" },
		{ IOEvent::Type::UNSUPPORTED, "Unsupported Action" }
	};
}