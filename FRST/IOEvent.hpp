#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <tuple>
#include <unordered_map>


#define IS_MOUSE_MOTION_EVENT(event) (event == MS_MOVE)
#define IS_MOUSE_BUTTON_EVENT(event) (event >= MS_START && event < MS_END)
#define IS_MOUSE_WHEEL_EVENT(event) (event == MS_WHEEL)
#define IS_KEYBOARD_EVENT(event) (event >= KB_START && event < KB_END)
#define IS_CONTROLLER_BUTTON_EVENT(event) (event >= CTRL_BUTTONS_START && event < CTRL_BUTTONS_END)
#define IS_CONTROLLER_AXIS_EVENT(event) (event >= CTRL_AXIS_START && event < CTRL_AXIS_END)


namespace FRST {
	namespace IO {
		class IOEvent {
		public:
			/*
			 * This is a general purpose class for handling all types of events.
			 * Many of the fields will be used differently for different types of events.
			 * Events should be handled this way only for aggregation of events during a frame
			 * to discern the control state and changes for that frame.
			 */

			// Use an alias to make input to getState more clear
			// A negative index indicates keyboard and mouse
			// >= 0 means a controller index
			typedef int Controller;

			enum Type;
			static const std::unordered_map<Type, const std::string> TypeLabels;
			static const std::string& getTypeString(Type type);

			struct Control {
				// The specific key or other type of event this is
				Type type;

				// Field for the control method that sent this event
				// Either -1 for the default or an sdl joystick index
				Controller controller;
			} control;

			// Only valid for movement type events
			// Like mouse movement or controller axis events
			int x;
			int y;
			// Relative fields will not be populated for controller axis events
			int dx;
			int dy;

			// Pressed or unpressed for buttons
			bool active;

			// Create an IOEvent from an SDL_Event
			IOEvent(SDL_Event* event);
			// Create a default state IOEvent
			IOEvent(Control control);
			// Make a copy of another IOEvent
			IOEvent(const IOEvent& other);

			// A boolean check for whether this state with no changes should be carried
			// over to the next frame.
			// Essentially if this event is a transition to a default state (or already
			// sitting at the default state) then this will return false
			inline bool shouldMoveToNextFrame() const;
		protected:
			// Used during the constructor to translate the event from an SDL event to an internal type
			void translateFromSDL(SDL_Event* event);
			void becomeControllerAxis(SDL_ControllerAxisEvent& event);
			void becomeControllerButton(SDL_ControllerButtonEvent& event);
			void becomeKey(SDL_KeyboardEvent& event);
			void becomeMouseMove(SDL_MouseMotionEvent& event);
			void becomeMouseButton(SDL_MouseButtonEvent& event);
			void becomeMouseWheel(SDL_MouseWheelEvent& event);
			void becomeQuit(SDL_QuitEvent& event);
			void becomeWindow(SDL_WindowEvent& event);
			void becomeUnsupported(SDL_Event& event);

		public:
			enum Type {
				/*
				 * All events are enumerated in this way to provide quick determination of which event occurred
				 */
				 // For Iteration
				ENUM_START,

				// Mouse Events
				MS_START = ENUM_START,
				MS_B1 = MS_START,
				MS_B2,
				MS_B3,
				MS_B4,
				MS_B5,
				MS_B6,
				MS_B7,
				MS_B8,
				MS_B9,
				MS_B10,
				MS_B11,
				MS_B12,
				MS_B13,
				MS_B14,
				MS_B15,
				MS_B16,
				MS_END,
				// If people need more mouse buttons than this then a better system should be thought up
				MS_WHEEL = MS_END,
				MS_MOVE,

				// Keyboard Events
				// One to one mapping of "normal" keyboard keys.
				// This would probably be a big issue for localization
				// F-keys
				KB_START,
				KB_F1 = KB_START,
				KB_F2,
				KB_F3,
				KB_F4,
				KB_F5,
				KB_F6,
				KB_F7,
				KB_F8,
				KB_F9,
				KB_F10,
				KB_F11,
				KB_F12, // Note: Windows-wide global GUI debug hotkey. Hits a breakpoint when debugging.
				KB_F13,
				KB_F14,
				KB_F15,
				KB_F16,
				KB_F17,
				KB_F18,
				KB_F19,
				KB_F20,
				KB_F21,
				KB_F22,
				KB_F23,
				KB_F24,
				// Number Keys
				KB_1,
				KB_2,
				KB_3,
				KB_4,
				KB_5,
				KB_6,
				KB_7,
				KB_8,
				KB_9,
				KB_0,
				// Alphabet Keys
				KB_A,
				KB_B,
				KB_C,
				KB_D,
				KB_E,
				KB_F,
				KB_G,
				KB_H,
				KB_I,
				KB_J,
				KB_K,
				KB_L,
				KB_M,
				KB_N,
				KB_O,
				KB_P,
				KB_Q,
				KB_R,
				KB_S,
				KB_T,
				KB_U,
				KB_V,
				KB_W,
				KB_X,
				KB_Y,
				KB_Z,
				// Everything Else
				KB_ALT_LEFT,
				KB_ALT_RIGHT,
				KB_APOSTROPHE,
				KB_ARROW_DOWN,
				KB_ARROW_LEFT,
				KB_ARROW_RIGHT,
				KB_ARROW_UP,
				KB_BACKSPACE,
				KB_BACKTICK, // `
				KB_BRACKET_LEFT, // [
				KB_BRACKET_RIGHT, // ]
				KB_CAPSLOCK,
				KB_COMMA,
				KB_CONTEXT_MENU, // That key on the right side beside the win key on some keyboards
				KB_CONTROL_LEFT,
				KB_CONTROL_RIGHT,
				KB_EQUALS,
				KB_ENTER,
				KB_ESC,
				KB_MINUS,
				KB_PERIOD,
				KB_SEMICOLON,
				KB_SHIFT_LEFT,
				KB_SHIFT_RIGHT,
				KB_SLASH_LEFT,
				KB_SLASH_RIGHT,
				KB_SPACE,
				KB_SUPER_LEFT, // Probably not useful in windows
				KB_SUPER_RIGHT,
				KB_TAB,
				KB_END,

				// Controller events
				// Buttons
				CTRL_BUTTONS_START = KB_END,
				CTRL_B_A = CTRL_BUTTONS_START,
				CTRL_B_B,
				CTRL_B_X,
				CTRL_B_Y,
				CTRL_B_BACK,
				CTRL_B_GUIDE,
				CTRL_B_START,
				CTRL_B_STICK_LEFT,
				CTRL_B_STICK_RIGHT,
				CTRL_B_SHOULDER_LEFT,
				CTRL_B_SHOULDER_RIGHT,
				CTRL_B_DPAD_DOWN,
				CTRL_B_DPAD_LEFT,
				CTRL_B_DPAD_RIGHT,
				CTRL_B_DPAD_UP,
				CTRL_BUTTONS_END,
				// Axis controls
				CTRL_AXIS_START = CTRL_BUTTONS_END,
				CTRL_AXIS_LEFT_X = CTRL_AXIS_START,
				CTRL_AXIS_LEFT_Y,
				CTRL_AXIS_RIGHT_X,
				CTRL_AXIS_RIGHT_Y,
				CTRL_AXIS_TRIGGER_LEFT,
				CTRL_AXIS_TRIGGER_RIGHT,
				CTRL_AXIS_END,
				// Adding and removing controllers
				CTRL_ADDED = CTRL_AXIS_END,
				CTRL_REMOVED,
				CTRL_REMAPPED,

				// Semantic Events
				SEMANTIC_START,
				QUIT = SEMANTIC_START,
				MINIMIZED,
				MAXIMIZED,
				RESTORED,
				MOUSE_FOCUS_GAINED,
				MOUSE_FOCUS_LOST,
				KEYBOARD_FOCUS_GAINED,
				KEYBOARD_FOCUS_LOST,
				// Do Nothing
				MOVE,
				CLOSE, // We have one window so we should never get this event
				// Unsupported
				RESIZE,
				MINIMIZE,
				FULLSCREEN,
				UNSUPPORTED,
				SEMANTIC_END,

				// For iteration
				ENUM_END = SEMANTIC_END,
			};
		};
	}
}