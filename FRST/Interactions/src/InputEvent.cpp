#include "Interactions/InputEvent.hpp"

#include <iostream>


namespace FRST {
	namespace Interactions {
		InputEvent::InputEvent(SDL_Event* event)
			: control{ Type::UNSUPPORTED, -1 }
			, x(0)
			, y(0)
			, dx(0)
			, dy(0)
			, active(false) {
			translateFromSDL(event);
		}

		InputEvent::InputEvent(Control control)
			: control(control)
			, x(0)
			, y(0)
			, dx(0)
			, dy(0)
			, active(false) {
		}

		InputEvent::InputEvent(const InputEvent& other)
			: control(other.control)
			, x(other.x)
			, y(other.y)
			, dx(0) // We don't copy change because it is no longer valid for the next frame
			, dy(0)
			, active(other.active) {
		}

		void InputEvent::translateFromSDL(SDL_Event* event) {
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

		template<class T> class TypeMapWithDefault : protected std::unordered_map<T, InputEvent::Type> {
		public:
			/*
			 * This template class is used to map SDL enums to InputEvent::Type enums with a default value
			 */
			TypeMapWithDefault(
				std::initializer_list<typename std::unordered_map<T, InputEvent::Type>::value_type> il)
				: std::unordered_map<T, InputEvent::Type>(il) {
				// Empty, just here to support list-initialization of the super
			}

			InputEvent::Type getType(T t) const {
				auto it = this->find(t);
				if (it == this->end()) {
					return InputEvent::Type::UNSUPPORTED;
				} else {
					return it->second;
				}
			}
		};

		static const TypeMapWithDefault<SDL_GameControllerAxis> controllerAxisToTypeMap = {
			{ SDL_CONTROLLER_AXIS_LEFTX, InputEvent::Type::CTRL_AXIS_LEFT_X },
			{ SDL_CONTROLLER_AXIS_LEFTY, InputEvent::Type::CTRL_AXIS_LEFT_Y },
			{ SDL_CONTROLLER_AXIS_RIGHTX, InputEvent::Type::CTRL_AXIS_RIGHT_X },
			{ SDL_CONTROLLER_AXIS_RIGHTY, InputEvent::Type::CTRL_AXIS_RIGHT_Y },
			{ SDL_CONTROLLER_AXIS_TRIGGERLEFT, InputEvent::Type::CTRL_AXIS_TRIGGER_LEFT },
			{ SDL_CONTROLLER_AXIS_TRIGGERRIGHT, InputEvent::Type::CTRL_AXIS_TRIGGER_RIGHT },
		};

		void InputEvent::becomeControllerAxis(SDL_ControllerAxisEvent& event) {
			// We have to cast here because event.axis holds an SDL_GameControllerAxis in the wrong datatype
			control.type = controllerAxisToTypeMap.getType((SDL_GameControllerAxis)event.axis);
			if (control.type == InputEvent::Type::UNSUPPORTED) {
				return;
			}

			control.controller = event.which;

			switch (control.type) {
			case InputEvent::Type::CTRL_AXIS_LEFT_X:
			case InputEvent::Type::CTRL_AXIS_RIGHT_X:
			case InputEvent::Type::CTRL_AXIS_TRIGGER_LEFT:
				x = event.value;
				break;
			case InputEvent::Type::CTRL_AXIS_LEFT_Y:
			case InputEvent::Type::CTRL_AXIS_RIGHT_Y:
			case InputEvent::Type::CTRL_AXIS_TRIGGER_RIGHT:
				y = event.value;
				break;
			default:
				// hmm
#ifdef _DEBUG2
				std::cout << "Encountered invallid controller axis type: " << control.type << std::endl;
#endif
				break;
			}
#ifdef _DEBUG2
			std::cout << "Controller Axis: " << getTypeString(type) << " " << event.value << std::endl;
#endif
		}

		static const TypeMapWithDefault<SDL_GameControllerButton> controllerButtonToTypeMap = {
			{ SDL_CONTROLLER_BUTTON_A, InputEvent::Type::CTRL_B_A },
			{ SDL_CONTROLLER_BUTTON_B, InputEvent::Type::CTRL_B_B },
			{ SDL_CONTROLLER_BUTTON_X, InputEvent::Type::CTRL_B_X },
			{ SDL_CONTROLLER_BUTTON_Y, InputEvent::Type::CTRL_B_Y },
			{ SDL_CONTROLLER_BUTTON_BACK, InputEvent::Type::CTRL_B_BACK },
			{ SDL_CONTROLLER_BUTTON_GUIDE, InputEvent::Type::CTRL_B_GUIDE },
			{ SDL_CONTROLLER_BUTTON_START, InputEvent::Type::CTRL_B_START },
			{ SDL_CONTROLLER_BUTTON_LEFTSTICK, InputEvent::Type::CTRL_B_STICK_LEFT },
			{ SDL_CONTROLLER_BUTTON_RIGHTSTICK, InputEvent::Type::CTRL_B_STICK_RIGHT },
			{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER, InputEvent::Type::CTRL_B_SHOULDER_LEFT },
			{ SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, InputEvent::Type::CTRL_B_SHOULDER_RIGHT },
			{ SDL_CONTROLLER_BUTTON_DPAD_DOWN, InputEvent::Type::CTRL_B_DPAD_DOWN },
			{ SDL_CONTROLLER_BUTTON_DPAD_LEFT, InputEvent::Type::CTRL_B_DPAD_LEFT },
			{ SDL_CONTROLLER_BUTTON_DPAD_RIGHT, InputEvent::Type::CTRL_B_DPAD_RIGHT },
			{ SDL_CONTROLLER_BUTTON_DPAD_UP, InputEvent::Type::CTRL_B_DPAD_UP },
		};

		void InputEvent::becomeControllerButton(SDL_ControllerButtonEvent& event) {
			control.type = controllerButtonToTypeMap.getType((SDL_GameControllerButton)event.button);
			if (control.type == InputEvent::Type::UNSUPPORTED) {
				return;
			}

			control.controller = event.which;
			active = event.state == SDL_PRESSED;
#ifdef _DEBUG
			std::cout << "Controller Button: " << getTypeString(control.type) << " " << active << std::endl;
#endif
		}

		void InputEvent::becomeControllerModification(SDL_ControllerDeviceEvent& event) {
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
			{ SDLK_F1, InputEvent::Type::KB_F1 },
			{ SDLK_F2, InputEvent::Type::KB_F2 },
			{ SDLK_F3, InputEvent::Type::KB_F3 },
			{ SDLK_F4, InputEvent::Type::KB_F4 },
			{ SDLK_F5, InputEvent::Type::KB_F5 },
			{ SDLK_F6, InputEvent::Type::KB_F6 },
			{ SDLK_F7, InputEvent::Type::KB_F7 },
			{ SDLK_F8, InputEvent::Type::KB_F8 },
			{ SDLK_F9, InputEvent::Type::KB_F9 },
			{ SDLK_F10, InputEvent::Type::KB_F10 },
			{ SDLK_F11, InputEvent::Type::KB_F11 },
			{ SDLK_F12, InputEvent::Type::KB_F12 },
			{ SDLK_F13, InputEvent::Type::KB_F13 },
			{ SDLK_F14, InputEvent::Type::KB_F14 },
			{ SDLK_F15, InputEvent::Type::KB_F15 },
			{ SDLK_F16, InputEvent::Type::KB_F16 },
			{ SDLK_F17, InputEvent::Type::KB_F17 },
			{ SDLK_F18, InputEvent::Type::KB_F18 },
			{ SDLK_F19, InputEvent::Type::KB_F19 },
			{ SDLK_F20, InputEvent::Type::KB_F20 },
			{ SDLK_F21, InputEvent::Type::KB_F21 },
			{ SDLK_F22, InputEvent::Type::KB_F22 },
			{ SDLK_F23, InputEvent::Type::KB_F23 },
			{ SDLK_F24, InputEvent::Type::KB_F24 },

			{ SDLK_1, InputEvent::Type::KB_1 },
			{ SDLK_2, InputEvent::Type::KB_2 },
			{ SDLK_3, InputEvent::Type::KB_3 },
			{ SDLK_4, InputEvent::Type::KB_4 },
			{ SDLK_5, InputEvent::Type::KB_5 },
			{ SDLK_6, InputEvent::Type::KB_6 },
			{ SDLK_7, InputEvent::Type::KB_7 },
			{ SDLK_8, InputEvent::Type::KB_8 },
			{ SDLK_9, InputEvent::Type::KB_9 },
			{ SDLK_0, InputEvent::Type::KB_0 },

			{ SDLK_a, InputEvent::Type::KB_A },
			{ SDLK_b, InputEvent::Type::KB_B },
			{ SDLK_c, InputEvent::Type::KB_C },
			{ SDLK_d, InputEvent::Type::KB_D },
			{ SDLK_e, InputEvent::Type::KB_E },
			{ SDLK_f, InputEvent::Type::KB_F },
			{ SDLK_g, InputEvent::Type::KB_G },
			{ SDLK_h, InputEvent::Type::KB_H },
			{ SDLK_i, InputEvent::Type::KB_I },
			{ SDLK_j, InputEvent::Type::KB_J },
			{ SDLK_k, InputEvent::Type::KB_K },
			{ SDLK_l, InputEvent::Type::KB_L },
			{ SDLK_m, InputEvent::Type::KB_M },
			{ SDLK_n, InputEvent::Type::KB_N },
			{ SDLK_o, InputEvent::Type::KB_O },
			{ SDLK_p, InputEvent::Type::KB_P },
			{ SDLK_q, InputEvent::Type::KB_Q },
			{ SDLK_r, InputEvent::Type::KB_R },
			{ SDLK_s, InputEvent::Type::KB_S },
			{ SDLK_t, InputEvent::Type::KB_T },
			{ SDLK_u, InputEvent::Type::KB_U },
			{ SDLK_v, InputEvent::Type::KB_V },
			{ SDLK_w, InputEvent::Type::KB_W },
			{ SDLK_x, InputEvent::Type::KB_X },
			{ SDLK_y, InputEvent::Type::KB_Y },
			{ SDLK_z, InputEvent::Type::KB_Z },

			{ SDLK_LALT, InputEvent::Type::KB_ALT_LEFT },
			{ SDLK_RALT, InputEvent::Type::KB_ALT_RIGHT },
			{ SDLK_QUOTE, InputEvent::Type::KB_APOSTROPHE },
			{ SDLK_DOWN, InputEvent::Type::KB_ARROW_DOWN },
			{ SDLK_LEFT, InputEvent::Type::KB_ARROW_LEFT },
			{ SDLK_RIGHT, InputEvent::Type::KB_ARROW_RIGHT },
			{ SDLK_UP, InputEvent::Type::KB_ARROW_UP },
			{ SDLK_BACKSPACE, InputEvent::Type::KB_BACKSPACE },
			{ SDLK_BACKQUOTE, InputEvent::Type::KB_BACKTICK },
			{ SDLK_LEFTBRACKET, InputEvent::Type::KB_BRACKET_LEFT },
			{ SDLK_RIGHTBRACKET, InputEvent::Type::KB_BRACKET_RIGHT },
			{ SDLK_CAPSLOCK, InputEvent::Type::KB_CAPSLOCK },
			{ SDLK_COMMA, InputEvent::Type::KB_COMMA },
			{ SDLK_APPLICATION, InputEvent::Type::KB_CONTEXT_MENU },
			{ SDLK_LCTRL, InputEvent::Type::KB_CONTROL_LEFT },
			{ SDLK_RCTRL, InputEvent::Type::KB_CONTROL_RIGHT },
			{ SDLK_EQUALS, InputEvent::Type::KB_EQUALS },
			{ SDLK_RETURN, InputEvent::Type::KB_ENTER },
			{ SDLK_ESCAPE, InputEvent::Type::KB_ESC },
			{ SDLK_MINUS, InputEvent::Type::KB_MINUS },
			{ SDLK_PERIOD, InputEvent::Type::KB_PERIOD },
			{ SDLK_SEMICOLON, InputEvent::Type::KB_SEMICOLON },
			{ SDLK_LSHIFT, InputEvent::Type::KB_SHIFT_LEFT },
			{ SDLK_RSHIFT, InputEvent::Type::KB_SHIFT_RIGHT },
			{ SDLK_BACKSLASH, InputEvent::Type::KB_SLASH_LEFT },
			{ SDLK_SLASH, InputEvent::Type::KB_SLASH_RIGHT },
			{ SDLK_SPACE, InputEvent::Type::KB_SPACE },
			{ SDLK_LGUI, InputEvent::Type::KB_SUPER_LEFT },
			{ SDLK_RGUI, InputEvent::Type::KB_SUPER_RIGHT },
			{ SDLK_TAB, InputEvent::Type::KB_TAB },
		};

		void InputEvent::becomeKey(SDL_KeyboardEvent& event) {
			// Ignore repeat keys for now
			if (event.repeat > 0) {
				control.type = InputEvent::Type::UNSUPPORTED;
				return;
			}
			// We decide keyboard keys based on keycode instead of scancode
			// This is just in case of future support of other keyboard layouts
			// Eg. A user (or OS) remapping another key (or setting another keyboard layout) will see the key pressed
			// as it's mapped, instead of as it physically is on a QWERTY keyboard
			control.type = keyToTypeMap.getType(event.keysym.sym);
			if (control.type == InputEvent::Type::UNSUPPORTED) {
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

		void InputEvent::becomeMouseMove(SDL_MouseMotionEvent& event) {
			control.type = Type::MS_MOVE;
			x = event.x;
			y = event.y;
			dx = event.xrel;
			dy = event.yrel;
#ifdef _DEBUG2
			std::cout << "Mouse Move: " << x << " " << y << " " << dx << " " << dy << std::endl;
#endif
		}

		static const InputEvent::Type mouseButtonToTypeMap[] = {
			InputEvent::Type::MS_B1,
			InputEvent::Type::MS_B2,
			InputEvent::Type::MS_B3,
			InputEvent::Type::MS_B4,
			InputEvent::Type::MS_B5,
			InputEvent::Type::MS_B6,
			InputEvent::Type::MS_B7,
			InputEvent::Type::MS_B8,
			InputEvent::Type::MS_B9,
			InputEvent::Type::MS_B10,
			InputEvent::Type::MS_B11,
			InputEvent::Type::MS_B12,
			InputEvent::Type::MS_B13,
			InputEvent::Type::MS_B14,
			InputEvent::Type::MS_B15,
			InputEvent::Type::MS_B16,
		};

		void InputEvent::becomeMouseButton(SDL_MouseButtonEvent& event) {
			control.type = mouseButtonToTypeMap[event.button - 1];
			x = event.x;
			y = event.y;
			active = event.state == SDL_PRESSED;
#ifdef _DEBUG
			std::cout << "Mouse Click: " << getTypeString(control.type) << " " << x << " " << y << " " << active << std::endl;
#endif
		}

		void InputEvent::becomeMouseWheel(SDL_MouseWheelEvent& event) {
			control.type = Type::MS_WHEEL;
			x = event.x;
			y = event.y;
#ifdef _DEBUG2
			std::cout << "Mouse Wheel: " << x << " " << y << std::endl;
#endif
		}

		void InputEvent::becomeQuit(SDL_QuitEvent& event) {
			control.type = Type::QUIT;
#ifdef _DEBUG
			std::cout << "QUIT" << std::endl;
#endif
		}

		void InputEvent::becomeWindow(SDL_WindowEvent& event) {
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

		void InputEvent::becomeUnsupported(SDL_Event& event) {
			control.type = Type::UNSUPPORTED;
#ifdef _DEBUG
			std::cout << "Unsupported: " << event.type << std::endl;
#endif
		}

		const std::string& InputEvent::getTypeString(Type type) {
			auto it = TypeLabels.find(type);

			if (it == TypeLabels.end()) {
				it = TypeLabels.find(Type::UNSUPPORTED);
				return it->second;
			}
			else {
				return it->second;
			}
		}

		const std::unordered_map<InputEvent::Type, const std::string> InputEvent::TypeLabels = {
			{ InputEvent::Type::MS_B1, "Left Mouse Button" },
			{ InputEvent::Type::MS_B2, "Middle Mouse Button" },
			{ InputEvent::Type::MS_B3, "Right Mouse Button" },
			{ InputEvent::Type::MS_B4, "Mouse Button 4" },
			{ InputEvent::Type::MS_B5, "Mouse Button 5" },
			{ InputEvent::Type::MS_B6, "Mouse Button 6" },
			{ InputEvent::Type::MS_B7, "Mouse Button 7" },
			{ InputEvent::Type::MS_B8, "Mouse Button 8" },
			{ InputEvent::Type::MS_B9, "Mouse Button 9" },
			{ InputEvent::Type::MS_B10, "Mouse Button 10" },
			{ InputEvent::Type::MS_B11, "Mouse Button 11" },
			{ InputEvent::Type::MS_B12, "Mouse Button 12" },
			{ InputEvent::Type::MS_B13, "Mouse Button 13" },
			{ InputEvent::Type::MS_B14, "Mouse Button 14" },
			{ InputEvent::Type::MS_B15, "Mouse Button 15" },
			{ InputEvent::Type::MS_B16, "Mouse Button 16" },
			{ InputEvent::Type::MS_WHEEL, "Mouse Wheel" },
			{ InputEvent::Type::MS_MOVE, "Mouse Move" },

			// TODO Should we include two strings, one short and one long?
			{ InputEvent::Type::KB_F1, "F1" },
			{ InputEvent::Type::KB_F2, "F2 Key" },
			{ InputEvent::Type::KB_F3, "F3 Key" },
			{ InputEvent::Type::KB_F4, "F4 Key" },
			{ InputEvent::Type::KB_F5, "F5 Key" },
			{ InputEvent::Type::KB_F6, "F6 Key" },
			{ InputEvent::Type::KB_F7, "F7 Key" },
			{ InputEvent::Type::KB_F8, "F8 Key" },
			{ InputEvent::Type::KB_F9, "F9 Key" },
			{ InputEvent::Type::KB_F10, "F10 Key" },
			{ InputEvent::Type::KB_F11, "F11 Key" },
			{ InputEvent::Type::KB_F12, "F12 Key" },
			{ InputEvent::Type::KB_F13, "F13 Key" },
			{ InputEvent::Type::KB_F14, "F14 Key" },
			{ InputEvent::Type::KB_F15, "F15 Key" },
			{ InputEvent::Type::KB_F16, "F16 Key" },
			{ InputEvent::Type::KB_F17, "F17 Key" },
			{ InputEvent::Type::KB_F18, "F18 Key" },
			{ InputEvent::Type::KB_F19, "F19 Key" },
			{ InputEvent::Type::KB_F20, "F20 Key" },
			{ InputEvent::Type::KB_F21, "F21 Key" },
			{ InputEvent::Type::KB_F22, "F22 Key" },
			{ InputEvent::Type::KB_F23, "F23 Key" },
			{ InputEvent::Type::KB_F24, "F24 Key" },
			{ InputEvent::Type::KB_1, "1 Key" },
			{ InputEvent::Type::KB_2, "2 Key" },
			{ InputEvent::Type::KB_3, "3 Key" },
			{ InputEvent::Type::KB_4, "4 Key" },
			{ InputEvent::Type::KB_5, "5 Key" },
			{ InputEvent::Type::KB_6, "6 Key" },
			{ InputEvent::Type::KB_7, "7 Key" },
			{ InputEvent::Type::KB_8, "8 Key" },
			{ InputEvent::Type::KB_9, "9 Key" },
			{ InputEvent::Type::KB_0, "0 Key" },
			{ InputEvent::Type::KB_A, "A Key" },
			{ InputEvent::Type::KB_B, "B Key" },
			{ InputEvent::Type::KB_C, "C Key" },
			{ InputEvent::Type::KB_D, "D Key" },
			{ InputEvent::Type::KB_E, "E Key" },
			{ InputEvent::Type::KB_F, "F Key" },
			{ InputEvent::Type::KB_G, "G Key" },
			{ InputEvent::Type::KB_H, "H Key" },
			{ InputEvent::Type::KB_I, "I Key" },
			{ InputEvent::Type::KB_J, "J Key" },
			{ InputEvent::Type::KB_K, "K Key" },
			{ InputEvent::Type::KB_L, "L Key" },
			{ InputEvent::Type::KB_M, "M Key" },
			{ InputEvent::Type::KB_N, "N Key" },
			{ InputEvent::Type::KB_O, "O Key" },
			{ InputEvent::Type::KB_P, "P Key" },
			{ InputEvent::Type::KB_Q, "Q Key" },
			{ InputEvent::Type::KB_R, "R Key" },
			{ InputEvent::Type::KB_S, "S Key" },
			{ InputEvent::Type::KB_T, "T Key" },
			{ InputEvent::Type::KB_U, "U Key" },
			{ InputEvent::Type::KB_V, "V Key" },
			{ InputEvent::Type::KB_W, "W Key" },
			{ InputEvent::Type::KB_X, "X Key" },
			{ InputEvent::Type::KB_Y, "Y Key" },
			{ InputEvent::Type::KB_Z, "Z Key" },

			{ InputEvent::Type::KB_ALT_LEFT, "Left Alt Key" },
			{ InputEvent::Type::KB_ALT_RIGHT, "Right Alt Key" },
			{ InputEvent::Type::KB_APOSTROPHE, "'" },
			{ InputEvent::Type::KB_ARROW_DOWN, "Down Arrow Key" },
			{ InputEvent::Type::KB_ARROW_LEFT, "Left Arrow Key" },
			{ InputEvent::Type::KB_ARROW_RIGHT, "Right Arrow Key" },
			{ InputEvent::Type::KB_ARROW_UP, "Up Arrow Key" },
			{ InputEvent::Type::KB_BACKSPACE, "Backspace Key" },
			{ InputEvent::Type::KB_BACKTICK, "` Key" },
			{ InputEvent::Type::KB_BRACKET_LEFT, "[ Key" },
			{ InputEvent::Type::KB_BRACKET_RIGHT, "] Key" },
			{ InputEvent::Type::KB_CAPSLOCK, "Caps Lock Key" },
			{ InputEvent::Type::KB_COMMA, ", Key" },
			{ InputEvent::Type::KB_CONTEXT_MENU, "That Other Weird key" },
			{ InputEvent::Type::KB_CONTROL_LEFT, "Left Control Key" },
			{ InputEvent::Type::KB_CONTROL_RIGHT, "Right Control Key" },
			{ InputEvent::Type::KB_EQUALS, "= Key" },
			{ InputEvent::Type::KB_ENTER, "Enter Key" },
			{ InputEvent::Type::KB_ESC, "Escape Key" },
			{ InputEvent::Type::KB_MINUS, "- Key" },
			{ InputEvent::Type::KB_PERIOD, ". Key" },
			{ InputEvent::Type::KB_SEMICOLON, "; Key" },
			{ InputEvent::Type::KB_SHIFT_LEFT, "Left Shift Key" },
			{ InputEvent::Type::KB_SHIFT_RIGHT, "Right Shift Key" },
			{ InputEvent::Type::KB_SLASH_LEFT, "\\ Key" },
			{ InputEvent::Type::KB_SLASH_RIGHT, "/ Key" },
			{ InputEvent::Type::KB_SPACE, "Space Key" },
			{ InputEvent::Type::KB_SUPER_LEFT, "Left Super Key" },
			{ InputEvent::Type::KB_SUPER_RIGHT, "Right Super Key" },
			{ InputEvent::Type::KB_TAB, "Tab Key" },

			{ InputEvent::Type::CTRL_B_A, "A Button" },
			{ InputEvent::Type::CTRL_B_B, "B Button" },
			{ InputEvent::Type::CTRL_B_X, "X Button" },
			{ InputEvent::Type::CTRL_B_Y, "Y Button" },
			{ InputEvent::Type::CTRL_B_BACK, "Controller Back Button" },
			{ InputEvent::Type::CTRL_B_GUIDE, "Controller Guide Button" },
			{ InputEvent::Type::CTRL_B_START, "Start Button" },
			{ InputEvent::Type::CTRL_B_STICK_LEFT, "Left Stick" },
			{ InputEvent::Type::CTRL_B_STICK_RIGHT, "Right Stick" },
			{ InputEvent::Type::CTRL_B_SHOULDER_LEFT, "Left Shoulder Button" },
			{ InputEvent::Type::CTRL_B_SHOULDER_RIGHT, "Right Shoulder Button" },
			{ InputEvent::Type::CTRL_B_DPAD_DOWN, "DPAD Down" },
			{ InputEvent::Type::CTRL_B_DPAD_LEFT, "DPAD Left" },
			{ InputEvent::Type::CTRL_B_DPAD_RIGHT, "DPAD Right" },
			{ InputEvent::Type::CTRL_B_DPAD_UP, "DPAD Up" },
			{ InputEvent::Type::CTRL_AXIS_LEFT_X, "Left Stick X Axis" },
			{ InputEvent::Type::CTRL_AXIS_LEFT_Y, "Left Stick Y Axis" },
			{ InputEvent::Type::CTRL_AXIS_RIGHT_X, "Right Stick X Axis" },
			{ InputEvent::Type::CTRL_AXIS_RIGHT_Y, "Right Stick Y Axis" },
			{ InputEvent::Type::CTRL_AXIS_TRIGGER_LEFT, "Left Trigger Axis" },
			{ InputEvent::Type::CTRL_AXIS_TRIGGER_RIGHT, "Right Trigger Axis" },
			{ InputEvent::Type::CTRL_ADDED, "Controller Added" },
			{ InputEvent::Type::CTRL_REMOVED, "Controller Removed" },
			{ InputEvent::Type::CTRL_REMAPPED, "Controller Remapped" },

			{ InputEvent::Type::QUIT, "Quit Application" },
			{ InputEvent::Type::WINDOW_MINIMIZED, "Window Minimized" },
			{ InputEvent::Type::WINDOW_MAXIMIZED, "Window Maximized" },
			{ InputEvent::Type::WINDOW_RESTORED, "Window Restored" },
			{ InputEvent::Type::MOUSE_FOCUS_GAINED, "Mouse Focus Gained" },
			{ InputEvent::Type::MOUSE_FOCUS_LOST, "Mouse Focus Lost" },
			{ InputEvent::Type::KEYBOARD_FOCUS_GAINED, "Keyboard Focus Gained" },
			{ InputEvent::Type::KEYBOARD_FOCUS_LOST, "Keyboard Focus Lost" },

			{ InputEvent::Type::WINDOW_MOVED, "Window Moved" },
			{ InputEvent::Type::WINDOW_CLOSED, "Window Closed" },
			{ InputEvent::Type::WINDOW_RESIZED, "Window Resized" },
			{ InputEvent::Type::WINDOW_MINIMIZED, "Window Minimized" },
			{ InputEvent::Type::WINDOW_FULLSCREEN, "Window Fullscreened" },
			{ InputEvent::Type::UNSUPPORTED, "Unsupported Action" }
		};
	}
}