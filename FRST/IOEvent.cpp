#include "IOEvent.hpp"

#include <iostream>


namespace FRST {
	namespace IO {
		IOEvent::IOEvent(SDL_Event* event)
			: control{ Type::UNSUPPORTED, -1 }
			, x(0)
			, y(0)
			, dx(0)
			, dy(0)
			, active(false) {
			translateFromSDL(event);
		}

		IOEvent::IOEvent(Control control) 
			: control(control)
			, x(0)
			, y(0)
			, dx(0)
			, dy(0)
			, active(false) {
		}

		IOEvent::IOEvent(const IOEvent& other)
			: control(other.control)
			, x(other.x)
			, y(other.y)
			, dx(0) // We don't copy change because it is no longer valid for the next frame
			, dy(0)
			, active(other.active) {
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

			case SDL_CONTROLLERDEVICEADDED:
			case SDL_CONTROLLERDEVICEREMAPPED:
			case SDL_CONTROLLERDEVICEREMOVED:
				becomeControllerModification(event->cdevice);
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

		template<class T> class TypeMapWithDefault : private std::unordered_map<T, IOEvent::Type> {
		public:
			/*
			 * This template class is used to map SDL enums to IOEvent::Type enums with a default value
			 */
			TypeMapWithDefault(
				std::initializer_list<typename std::unordered_map<T, IOEvent::Type>::value_type> il)
				: std::unordered_map<T, IOEvent::Type>(il) {
				// Empty, just here to support list-initialization of the super
			}

			IOEvent::Type getType(T t) const {
				auto it = find(t);
				if (it == end()) {
					return IOEvent::Type::UNSUPPORTED;
				}
				else {
					return it->second;
				}
			}
		};

		static const TypeMapWithDefault<SDL_GameControllerAxis> controllerAxisToTypeMap = {
			{ SDL_CONTROLLER_AXIS_LEFTX, IOEvent::Type::CTRL_AXIS_LEFT_X },
			{ SDL_CONTROLLER_AXIS_LEFTY, IOEvent::Type::CTRL_AXIS_LEFT_Y },
			{ SDL_CONTROLLER_AXIS_RIGHTX, IOEvent::Type::CTRL_AXIS_RIGHT_X },
			{ SDL_CONTROLLER_AXIS_RIGHTY, IOEvent::Type::CTRL_AXIS_RIGHT_Y },
			{ SDL_CONTROLLER_AXIS_TRIGGERLEFT, IOEvent::Type::CTRL_AXIS_TRIGGER_LEFT },
			{ SDL_CONTROLLER_AXIS_TRIGGERRIGHT, IOEvent::Type::CTRL_AXIS_TRIGGER_RIGHT },
		};

		void IOEvent::becomeControllerAxis(SDL_ControllerAxisEvent& event) {
			// We have to cast here because event.axis holds an SDL_GameControllerAxis in the wrong datatype
			control.type = controllerAxisToTypeMap.getType((SDL_GameControllerAxis)event.axis);
			if (control.type == IOEvent::Type::UNSUPPORTED) {
				return;
			}

			control.controller = event.which;

			switch (control.type) {
			case IOEvent::Type::CTRL_AXIS_LEFT_X:
			case IOEvent::Type::CTRL_AXIS_RIGHT_X:
			case IOEvent::Type::CTRL_AXIS_TRIGGER_LEFT:
				x = event.value;
				break;
			case IOEvent::Type::CTRL_AXIS_LEFT_Y:
			case IOEvent::Type::CTRL_AXIS_RIGHT_Y:
			case IOEvent::Type::CTRL_AXIS_TRIGGER_RIGHT:
				y = event.value;
				break;
			}
#ifdef _DEBUG2
			std::cout << "Controller Axis: " << getTypeString(type) << " " << event.value << std::endl;
#endif
		}

		static const TypeMapWithDefault<SDL_GameControllerButton> controllerButtonToTypeMap = {
			{ SDL_CONTROLLER_BUTTON_A, IOEvent::Type::CTRL_B_A },
			{ SDL_CONTROLLER_BUTTON_B, IOEvent::Type::CTRL_B_B },
			{ SDL_CONTROLLER_BUTTON_X, IOEvent::Type::CTRL_B_X },
			{ SDL_CONTROLLER_BUTTON_Y, IOEvent::Type::CTRL_B_Y },
			{ SDL_CONTROLLER_BUTTON_BACK, IOEvent::Type::CTRL_B_BACK },
			{ SDL_CONTROLLER_BUTTON_GUIDE, IOEvent::Type::CTRL_B_GUIDE },
			{ SDL_CONTROLLER_BUTTON_START, IOEvent::Type::CTRL_B_START },
			{ SDL_CONTROLLER_BUTTON_LEFTSTICK, IOEvent::Type::CTRL_B_STICK_LEFT },
			{ SDL_CONTROLLER_BUTTON_RIGHTSTICK, IOEvent::Type::CTRL_B_STICK_RIGHT },
			{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER, IOEvent::Type::CTRL_B_SHOULDER_LEFT },
			{ SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, IOEvent::Type::CTRL_B_SHOULDER_RIGHT },
			{ SDL_CONTROLLER_BUTTON_DPAD_DOWN, IOEvent::Type::CTRL_B_DPAD_DOWN },
			{ SDL_CONTROLLER_BUTTON_DPAD_LEFT, IOEvent::Type::CTRL_B_DPAD_LEFT },
			{ SDL_CONTROLLER_BUTTON_DPAD_RIGHT, IOEvent::Type::CTRL_B_DPAD_RIGHT },
			{ SDL_CONTROLLER_BUTTON_DPAD_UP, IOEvent::Type::CTRL_B_DPAD_UP },
		};

		void IOEvent::becomeControllerButton(SDL_ControllerButtonEvent& event) {
			control.type = controllerButtonToTypeMap.getType((SDL_GameControllerButton)event.button);
			if (control.type == IOEvent::Type::UNSUPPORTED) {
				return;
			}

			control.controller = event.which;
			active = event.state == SDL_PRESSED;
#ifdef _DEBUG
			std::cout << "Controller Button: " << getTypeString(control.type) << " " << active << std::endl;
#endif
		}

		void IOEvent::becomeControllerModification(SDL_ControllerDeviceEvent& event) {
			switch (event.type) {
			case SDL_CONTROLLERDEVICEADDED:
				control.type = Type::CTRL_ADDED;
				break;
			case SDL_CONTROLLERDEVICEREMAPPED:
				control.type = Type::CTRL_REMAPPED;
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				control.type = Type::CTRL_REMOVED;
				break;
			}

			control.controller = event.which;
		}

		static const TypeMapWithDefault<SDL_Keycode> keyToTypeMap = {
			{ SDLK_F1, IOEvent::Type::KB_F1 },
			{ SDLK_F2, IOEvent::Type::KB_F2 },
			{ SDLK_F3, IOEvent::Type::KB_F3 },
			{ SDLK_F4, IOEvent::Type::KB_F4 },
			{ SDLK_F5, IOEvent::Type::KB_F5 },
			{ SDLK_F6, IOEvent::Type::KB_F6 },
			{ SDLK_F7, IOEvent::Type::KB_F7 },
			{ SDLK_F8, IOEvent::Type::KB_F8 },
			{ SDLK_F9, IOEvent::Type::KB_F9 },
			{ SDLK_F10, IOEvent::Type::KB_F10 },
			{ SDLK_F11, IOEvent::Type::KB_F11 },
			{ SDLK_F12, IOEvent::Type::KB_F12 },
			{ SDLK_F13, IOEvent::Type::KB_F13 },
			{ SDLK_F14, IOEvent::Type::KB_F14 },
			{ SDLK_F15, IOEvent::Type::KB_F15 },
			{ SDLK_F16, IOEvent::Type::KB_F16 },
			{ SDLK_F17, IOEvent::Type::KB_F17 },
			{ SDLK_F18, IOEvent::Type::KB_F18 },
			{ SDLK_F19, IOEvent::Type::KB_F19 },
			{ SDLK_F20, IOEvent::Type::KB_F20 },
			{ SDLK_F21, IOEvent::Type::KB_F21 },
			{ SDLK_F22, IOEvent::Type::KB_F22 },
			{ SDLK_F23, IOEvent::Type::KB_F23 },
			{ SDLK_F24, IOEvent::Type::KB_F24 },

			{ SDLK_1, IOEvent::Type::KB_1 },
			{ SDLK_2, IOEvent::Type::KB_2 },
			{ SDLK_3, IOEvent::Type::KB_3 },
			{ SDLK_4, IOEvent::Type::KB_4 },
			{ SDLK_5, IOEvent::Type::KB_5 },
			{ SDLK_6, IOEvent::Type::KB_6 },
			{ SDLK_7, IOEvent::Type::KB_7 },
			{ SDLK_8, IOEvent::Type::KB_8 },
			{ SDLK_9, IOEvent::Type::KB_9 },
			{ SDLK_0, IOEvent::Type::KB_0 },

			{ SDLK_a, IOEvent::Type::KB_A },
			{ SDLK_b, IOEvent::Type::KB_B },
			{ SDLK_c, IOEvent::Type::KB_C },
			{ SDLK_d, IOEvent::Type::KB_D },
			{ SDLK_e, IOEvent::Type::KB_E },
			{ SDLK_f, IOEvent::Type::KB_F },
			{ SDLK_g, IOEvent::Type::KB_G },
			{ SDLK_h, IOEvent::Type::KB_H },
			{ SDLK_i, IOEvent::Type::KB_I },
			{ SDLK_j, IOEvent::Type::KB_J },
			{ SDLK_k, IOEvent::Type::KB_K },
			{ SDLK_l, IOEvent::Type::KB_L },
			{ SDLK_m, IOEvent::Type::KB_M },
			{ SDLK_n, IOEvent::Type::KB_N },
			{ SDLK_o, IOEvent::Type::KB_O },
			{ SDLK_p, IOEvent::Type::KB_P },
			{ SDLK_q, IOEvent::Type::KB_Q },
			{ SDLK_r, IOEvent::Type::KB_R },
			{ SDLK_s, IOEvent::Type::KB_S },
			{ SDLK_t, IOEvent::Type::KB_T },
			{ SDLK_u, IOEvent::Type::KB_U },
			{ SDLK_v, IOEvent::Type::KB_V },
			{ SDLK_w, IOEvent::Type::KB_W },
			{ SDLK_x, IOEvent::Type::KB_X },
			{ SDLK_y, IOEvent::Type::KB_Y },
			{ SDLK_z, IOEvent::Type::KB_Z },

			{ SDLK_LALT, IOEvent::Type::KB_ALT_LEFT },
			{ SDLK_RALT, IOEvent::Type::KB_ALT_RIGHT },
			{ SDLK_QUOTE, IOEvent::Type::KB_APOSTROPHE },
			{ SDLK_DOWN, IOEvent::Type::KB_ARROW_DOWN },
			{ SDLK_LEFT, IOEvent::Type::KB_ARROW_LEFT },
			{ SDLK_RIGHT, IOEvent::Type::KB_ARROW_RIGHT },
			{ SDLK_UP, IOEvent::Type::KB_ARROW_UP },
			{ SDLK_BACKSPACE, IOEvent::Type::KB_BACKSPACE },
			{ SDLK_BACKQUOTE, IOEvent::Type::KB_BACKTICK },
			{ SDLK_LEFTBRACKET, IOEvent::Type::KB_BRACKET_LEFT },
			{ SDLK_RIGHTBRACKET, IOEvent::Type::KB_BRACKET_RIGHT },
			{ SDLK_CAPSLOCK, IOEvent::Type::KB_CAPSLOCK },
			{ SDLK_COMMA, IOEvent::Type::KB_COMMA },
			{ SDLK_APPLICATION, IOEvent::Type::KB_CONTEXT_MENU },
			{ SDLK_LCTRL, IOEvent::Type::KB_CONTROL_LEFT },
			{ SDLK_RCTRL, IOEvent::Type::KB_CONTROL_RIGHT },
			{ SDLK_EQUALS, IOEvent::Type::KB_EQUALS },
			{ SDLK_RETURN, IOEvent::Type::KB_ENTER },
			{ SDLK_ESCAPE, IOEvent::Type::KB_ESC },
			{ SDLK_MINUS, IOEvent::Type::KB_MINUS },
			{ SDLK_PERIOD, IOEvent::Type::KB_PERIOD },
			{ SDLK_SEMICOLON, IOEvent::Type::KB_SEMICOLON },
			{ SDLK_LSHIFT, IOEvent::Type::KB_SHIFT_LEFT },
			{ SDLK_RSHIFT, IOEvent::Type::KB_SHIFT_RIGHT },
			{ SDLK_BACKSLASH, IOEvent::Type::KB_SLASH_LEFT },
			{ SDLK_SLASH, IOEvent::Type::KB_SLASH_RIGHT },
			{ SDLK_SPACE, IOEvent::Type::KB_SPACE },
			{ SDLK_LGUI, IOEvent::Type::KB_SUPER_LEFT },
			{ SDLK_RGUI, IOEvent::Type::KB_SUPER_RIGHT },
			{ SDLK_TAB, IOEvent::Type::KB_TAB },
		};

		void IOEvent::becomeKey(SDL_KeyboardEvent& event) {
			// Ignore repeat keys for now
			if (event.repeat > 0) {
				control.type = IOEvent::Type::UNSUPPORTED;
				return;
			}
			// We decide keyboard keys based on keycode instead of scancode
			// This is just in case of future support of other keyboard layouts
			// Eg. A user (or OS) remapping another key (or setting another keyboard layout) will see the key pressed
			// as it's mapped, instead of as it physically is on a QWERTY keyboard
			control.type = keyToTypeMap.getType(event.keysym.sym);
			if (control.type == IOEvent::Type::UNSUPPORTED) {
#ifdef _DEBUG
				std::cout << "Unsupported key: " << event.keysym.sym << " " << event.keysym.scancode << " " << event.keysym.mod << " " << event.keysym.unused << std::endl;
#endif
				return;
			}

			active = event.state == SDL_PRESSED;
#ifdef _DEBUG
			std::cout << "Key: " << getTypeString(control.type) << " " << active << std::endl;
#endif

		}

		void IOEvent::becomeMouseMove(SDL_MouseMotionEvent& event) {
			control.type = Type::MS_MOVE;
			x = event.x;
			y = event.y;
			dx = event.xrel;
			dy = event.yrel;
#ifdef _DEBUG2
			std::cout << "Mouse Move: " << x << " " << y << " " << dx << " " << dy << std::endl;
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
			control.type = mouseButtonToTypeMap[event.button - 1];
			x = event.x;
			y = event.y;
			active = event.state == SDL_PRESSED;
#ifdef _DEBUG
			std::cout << "Mouse Click: " << getTypeString(control.type) << " " << x << " " << y << " " << active << std::endl;
#endif
		}

		void IOEvent::becomeMouseWheel(SDL_MouseWheelEvent& event) {
			control.type = Type::MS_WHEEL;
			x = event.x;
			y = event.y;
#ifdef _DEBUG2
			std::cout << "Mouse Wheel: " << x << " " << y << std::endl;
#endif
		}

		void IOEvent::becomeQuit(SDL_QuitEvent& event) {
			control.type = Type::QUIT;
#ifdef _DEBUG
			std::cout << "QUIT" << std::endl;
#endif
		}

		void IOEvent::becomeWindow(SDL_WindowEvent& event) {
			switch (event.type) {
			case SDL_WINDOWEVENT_MOVED:
				control.type = Type::WINDOW_MOVED;
				break;
			case SDL_WINDOWEVENT_RESIZED:
			// case SDL_WINDOWEVENT_SIZE_CHANGED:
				control.type = Type::WINDOW_RESIZED;
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				control.type = Type::WINDOW_MINIMIZED;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				control.type = Type::WINDOW_MAXIMIZED;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				control.type = Type::WINDOW_RESTORED;
				break;
			case SDL_WINDOWEVENT_ENTER:
				control.type = Type::MOUSE_FOCUS_GAINED;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				control.type = Type::MOUSE_FOCUS_LOST;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				control.type = Type::KEYBOARD_FOCUS_GAINED;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				control.type = Type::KEYBOARD_FOCUS_LOST;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				control.type = Type::WINDOW_CLOSED;
				break;
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_EXPOSED:
			case SDL_WINDOWEVENT_TAKE_FOCUS:
			case SDL_WINDOWEVENT_HIT_TEST:
			default:
				control.type = Type::UNSUPPORTED;
			}

			x = event.data1;
			y = event.data2;
		}

		void IOEvent::becomeUnsupported(SDL_Event& event) {
			control.type = Type::UNSUPPORTED;
#ifdef _DEBUG
			std::cout << "Unsupported: " << event.type << std::endl;
#endif
		}

		const std::string& IOEvent::getTypeString(Type type) {
			auto it = TypeLabels.find(type);

			if (it == TypeLabels.end()) {
				it = TypeLabels.find(Type::UNSUPPORTED);
				return it->second;
			}
			else {
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
			{ IOEvent::Type::KB_APOSTROPHE, "'" },
			{ IOEvent::Type::KB_ARROW_DOWN, "Down Arrow Key" },
			{ IOEvent::Type::KB_ARROW_LEFT, "Left Arrow Key" },
			{ IOEvent::Type::KB_ARROW_RIGHT, "Right Arrow Key" },
			{ IOEvent::Type::KB_ARROW_UP, "Up Arrow Key" },
			{ IOEvent::Type::KB_BACKSPACE, "Backspace Key" },
			{ IOEvent::Type::KB_BACKTICK, "` Key" },
			{ IOEvent::Type::KB_BRACKET_LEFT, "[ Key" },
			{ IOEvent::Type::KB_BRACKET_RIGHT, "] Key" },
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
			{ IOEvent::Type::KB_SEMICOLON, "; Key" },
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
			{ IOEvent::Type::WINDOW_MINIMIZED, "Window Minimized" },
			{ IOEvent::Type::WINDOW_MAXIMIZED, "Window Maximized" },
			{ IOEvent::Type::WINDOW_RESTORED, "Window Restored" },
			{ IOEvent::Type::MOUSE_FOCUS_GAINED, "Mouse Focus Gained" },
			{ IOEvent::Type::MOUSE_FOCUS_LOST, "Mouse Focus Lost" },
			{ IOEvent::Type::KEYBOARD_FOCUS_GAINED, "Keyboard Focus Gained" },
			{ IOEvent::Type::KEYBOARD_FOCUS_LOST, "Keyboard Focus Lost" },

			{ IOEvent::Type::WINDOW_MOVED, "Window Moved" },
			{ IOEvent::Type::WINDOW_CLOSED, "Window Closed" },
			{ IOEvent::Type::WINDOW_RESIZED, "Window Resized" },
			{ IOEvent::Type::WINDOW_MINIMIZED, "Window Minimized" },
			{ IOEvent::Type::WINDOW_FULLSCREEN, "Window Fullscreened" },
			{ IOEvent::Type::UNSUPPORTED, "Unsupported Action" }
		};
	}
}