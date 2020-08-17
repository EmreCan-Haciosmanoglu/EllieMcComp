#pragma once

namespace Can
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CAN_KEY_SPACE           ::Can::Key::Space
#define CAN_KEY_APOSTROPHE      ::Can::Key::Apostrophe    /* ' */
#define CAN_KEY_COMMA           ::Can::Key::Comma         /* , */
#define CAN_KEY_MINUS           ::Can::Key::Minus         /* - */
#define CAN_KEY_PERIOD          ::Can::Key::Period        /* . */
#define CAN_KEY_SLASH           ::Can::Key::Slash         /* / */
#define CAN_KEY_0               ::Can::Key::D0
#define CAN_KEY_1               ::Can::Key::D1
#define CAN_KEY_2               ::Can::Key::D2
#define CAN_KEY_3               ::Can::Key::D3
#define CAN_KEY_4               ::Can::Key::D4
#define CAN_KEY_5               ::Can::Key::D5
#define CAN_KEY_6               ::Can::Key::D6
#define CAN_KEY_7               ::Can::Key::D7
#define CAN_KEY_8               ::Can::Key::D8
#define CAN_KEY_9               ::Can::Key::D9
#define CAN_KEY_SEMICOLON       ::Can::Key::Semicolon     /* ; */
#define CAN_KEY_EQUAL           ::Can::Key::Equal         /* = */
#define CAN_KEY_A               ::Can::Key::A
#define CAN_KEY_B               ::Can::Key::B
#define CAN_KEY_C               ::Can::Key::C
#define CAN_KEY_D               ::Can::Key::D
#define CAN_KEY_E               ::Can::Key::E
#define CAN_KEY_F               ::Can::Key::F
#define CAN_KEY_G               ::Can::Key::G
#define CAN_KEY_H               ::Can::Key::H
#define CAN_KEY_I               ::Can::Key::I
#define CAN_KEY_J               ::Can::Key::J
#define CAN_KEY_K               ::Can::Key::K
#define CAN_KEY_L               ::Can::Key::L
#define CAN_KEY_M               ::Can::Key::M
#define CAN_KEY_N               ::Can::Key::N
#define CAN_KEY_O               ::Can::Key::O
#define CAN_KEY_P               ::Can::Key::P
#define CAN_KEY_Q               ::Can::Key::Q
#define CAN_KEY_R               ::Can::Key::R
#define CAN_KEY_S               ::Can::Key::S
#define CAN_KEY_T               ::Can::Key::T
#define CAN_KEY_U               ::Can::Key::U
#define CAN_KEY_V               ::Can::Key::V
#define CAN_KEY_W               ::Can::Key::W
#define CAN_KEY_X               ::Can::Key::X
#define CAN_KEY_Y               ::Can::Key::Y
#define CAN_KEY_Z               ::Can::Key::Z
#define CAN_KEY_LEFT_BRACKET    ::Can::Key::LeftBracket   /* [ */
#define CAN_KEY_BACKSLASH       ::Can::Key::Backslash     /* \ */
#define CAN_KEY_RIGHT_BRACKET   ::Can::Key::RightBracket  /* ] */
#define CAN_KEY_GRAVE_ACCENT    ::Can::Key::GraveAccent   /* ` */
#define CAN_KEY_WORLD_1         ::Can::Key::World1        /* non-US #1 */
#define CAN_KEY_WORLD_2         ::Can::Key::World2        /* non-US #2 */

/* Function keys */
#define CAN_KEY_ESCAPE          ::Can::Key::Escape
#define CAN_KEY_ENTER           ::Can::Key::Enter
#define CAN_KEY_TAB             ::Can::Key::Tab
#define CAN_KEY_BACKSPACE       ::Can::Key::Backspace
#define CAN_KEY_INSERT          ::Can::Key::Insert
#define CAN_KEY_DELETE          ::Can::Key::Delete
#define CAN_KEY_RIGHT           ::Can::Key::Right
#define CAN_KEY_LEFT            ::Can::Key::Left
#define CAN_KEY_DOWN            ::Can::Key::Down
#define CAN_KEY_UP              ::Can::Key::Up
#define CAN_KEY_PAGE_UP         ::Can::Key::PageUp
#define CAN_KEY_PAGE_DOWN       ::Can::Key::PageDown
#define CAN_KEY_HOME            ::Can::Key::Home
#define CAN_KEY_END             ::Can::Key::End
#define CAN_KEY_CAPS_LOCK       ::Can::Key::CapsLock
#define CAN_KEY_SCROLL_LOCK     ::Can::Key::ScrollLock
#define CAN_KEY_NUM_LOCK        ::Can::Key::NumLock
#define CAN_KEY_PRINT_SCREEN    ::Can::Key::PrintScreen
#define CAN_KEY_PAUSE           ::Can::Key::Pause
#define CAN_KEY_F1              ::Can::Key::F1
#define CAN_KEY_F2              ::Can::Key::F2
#define CAN_KEY_F3              ::Can::Key::F3
#define CAN_KEY_F4              ::Can::Key::F4
#define CAN_KEY_F5              ::Can::Key::F5
#define CAN_KEY_F6              ::Can::Key::F6
#define CAN_KEY_F7              ::Can::Key::F7
#define CAN_KEY_F8              ::Can::Key::F8
#define CAN_KEY_F9              ::Can::Key::F9
#define CAN_KEY_F10             ::Can::Key::F10
#define CAN_KEY_F11             ::Can::Key::F11
#define CAN_KEY_F12             ::Can::Key::F12
#define CAN_KEY_F13             ::Can::Key::F13
#define CAN_KEY_F14             ::Can::Key::F14
#define CAN_KEY_F15             ::Can::Key::F15
#define CAN_KEY_F16             ::Can::Key::F16
#define CAN_KEY_F17             ::Can::Key::F17
#define CAN_KEY_F18             ::Can::Key::F18
#define CAN_KEY_F19             ::Can::Key::F19
#define CAN_KEY_F20             ::Can::Key::F20
#define CAN_KEY_F21             ::Can::Key::F21
#define CAN_KEY_F22             ::Can::Key::F22
#define CAN_KEY_F23             ::Can::Key::F23
#define CAN_KEY_F24             ::Can::Key::F24
#define CAN_KEY_F25             ::Can::Key::F25

/* Keypad */
#define CAN_KEY_KP_0            ::Can::Key::KP0
#define CAN_KEY_KP_1            ::Can::Key::KP1
#define CAN_KEY_KP_2            ::Can::Key::KP2
#define CAN_KEY_KP_3            ::Can::Key::KP3
#define CAN_KEY_KP_4            ::Can::Key::KP4
#define CAN_KEY_KP_5            ::Can::Key::KP5
#define CAN_KEY_KP_6            ::Can::Key::KP6
#define CAN_KEY_KP_7            ::Can::Key::KP7
#define CAN_KEY_KP_8            ::Can::Key::KP8
#define CAN_KEY_KP_9            ::Can::Key::KP9
#define CAN_KEY_KP_DECIMAL      ::Can::Key::KPDecimal
#define CAN_KEY_KP_DIVIDE       ::Can::Key::KPDivide
#define CAN_KEY_KP_MULTIPLY     ::Can::Key::KPMultiply
#define CAN_KEY_KP_SUBTRACT     ::Can::Key::KPSubtract
#define CAN_KEY_KP_ADD          ::Can::Key::KPAdd
#define CAN_KEY_KP_ENTER        ::Can::Key::KPEnter
#define CAN_KEY_KP_EQUAL        ::Can::Key::KPEqual

#define CAN_KEY_LEFT_SHIFT      ::Can::Key::LeftShift
#define CAN_KEY_LEFT_CONTROL    ::Can::Key::LeftControl
#define CAN_KEY_LEFT_ALT        ::Can::Key::LeftAlt
#define CAN_KEY_LEFT_SUPER      ::Can::Key::LeftSuper
#define CAN_KEY_RIGHT_SHIFT     ::Can::Key::RightShift
#define CAN_KEY_RIGHT_CONTROL   ::Can::Key::RightControl
#define CAN_KEY_RIGHT_ALT       ::Can::Key::RightAlt
#define CAN_KEY_RIGHT_SUPER     ::Can::Key::RightSuper
#define CAN_KEY_MENU            ::Can::Key::Menu