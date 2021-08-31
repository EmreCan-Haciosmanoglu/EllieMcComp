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

	struct String_Keycode_Pair
	{
		KeyCode keycode;
		std::string string;
	};

	const String_Keycode_Pair strings[] = {

		{KeyCode::Space, "Space"}, // 32
		{},{},{},{},{},{},
		{KeyCode::Apostrophe, "Apostrophe"},//39, /* ' */
		{},{},{},{},
		{KeyCode::Comma, "Comma"}, //44, /* , */
		{KeyCode::Minus, "Minus"}, //45, /* - */
		{KeyCode::Period, "Period"}, //46, /* . */
		{KeyCode::Slash, "Slash"}, //47, /* / */
		{KeyCode::D0, "D0"}, //48, /* 0 */
		{KeyCode::D1, "D1"}, //49, /* 1 */
		{KeyCode::D2, "D2"}, //50, /* 2 */
		{KeyCode::D3, "D3"}, //51, /* 3 */
		{KeyCode::D4, "D4"}, //52, /* 4 */
		{KeyCode::D5, "D5"}, //53, /* 5 */
		{KeyCode::D6, "D6"}, //54, /* 6 */
		{KeyCode::D7, "D7"}, //55, /* 7 */
		{KeyCode::D8, "D8"}, //56, /* 8 */
		{KeyCode::D9, "D9"}, //57, /* 9 */
		{},
		{KeyCode::Semicolon, "Semicolon"}, //59, /* ; */
		{},
		{KeyCode::Equal, "Equal"}, //61, /* = */
		{},{},{},
		{KeyCode::A, "A"}, //65
		{KeyCode::B, "B"},
		{KeyCode::C, "C"},
		{KeyCode::D, "D"},
		{KeyCode::E, "E"},
		{KeyCode::F, "F"},
		{KeyCode::G, "G"},
		{KeyCode::H, "H"},
		{KeyCode::I, "I"},
		{KeyCode::J, "J"},
		{KeyCode::K, "K"},
		{KeyCode::L, "L"},
		{KeyCode::M, "M"},
		{KeyCode::N, "N"},
		{KeyCode::O, "O"},
		{KeyCode::P, "P"},
		{KeyCode::Q, "Q"},
		{KeyCode::R, "R"},
		{KeyCode::S, "S"},
		{KeyCode::T, "T"},
		{KeyCode::U, "U"},
		{KeyCode::V, "V"},
		{KeyCode::W, "W"},
		{KeyCode::X, "X"},
		{KeyCode::Y, "Y"},
		{KeyCode::Z, "Z"}, //90
		{KeyCode::LeftBracket, "LeftBracket"}, //91,  /* [ */
		{KeyCode::Backslash, "Backslash"}, //92,  /* \ */
		{KeyCode::RightBracket, "RightBracket"}, //93,  /* ] */
		{},{},
		{KeyCode::GraveAccent, "GraveAccent"}, //96,  /* ` */
		{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},
		{KeyCode::World1, "World1"}, //161, /* non-US #1 */
		{KeyCode::World2, "World2"}, //162, /* non-US #2 */
		{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},{},{},{},{},
		{},{},{},{},{},{},
		{KeyCode::Escape, "Escape"}, //256,
		{KeyCode::Enter, "Enter"}, //257,
		{KeyCode::Tab, "Tab"}, //258,
		{KeyCode::Backspace, "Backspace"}, //259,
		{KeyCode::Insert, "Insert"}, //260,
		{KeyCode::Delete, "Delete"}, //261,
		{KeyCode::Right, "Right"}, //262,
		{KeyCode::Left, "Left"}, //263,
		{KeyCode::Down, "Down"}, //264,
		{KeyCode::Up, "Up"}, //265,
		{KeyCode::PageUp, "PageUp"}, //266,
		{KeyCode::PageDown, "PageDown"}, //267,
		{KeyCode::Home, "Home"}, //268,
		{KeyCode::End, "End"}, //269,
		{},{},{},{},{},{},{},{},{},{},
		{KeyCode::CapsLock, "CapsLock"}, //280,
		{KeyCode::ScrollLock, "ScrollLock"}, //281,
		{KeyCode::NumLock, "NumLock"}, //282,
		{KeyCode::PrintScreen, "PrintScreen"}, //283,
		{KeyCode::Pause, "Pause"}, //284,
		{},{},{},{},{},
		{KeyCode::F1, "F1"}, //290,
		{KeyCode::F2, "F2"}, //291,
		{KeyCode::F3, "F3"}, //292,
		{KeyCode::F4, "F4"}, //293,
		{KeyCode::F5, "F5"}, //294,
		{KeyCode::F6, "F6"}, //295,
		{KeyCode::F7, "F7"}, //296,
		{KeyCode::F8, "F8"}, //297,
		{KeyCode::F9, "F9"}, //298,
		{KeyCode::F10, "F10"}, //299,
		{KeyCode::F11, "F11"}, //300,
		{KeyCode::F12, "F12"}, //301,
		{KeyCode::F13, "F13"}, //302,
		{KeyCode::F14, "F14"}, //303,
		{KeyCode::F15, "F15"}, //304,
		{KeyCode::F16, "F16"}, //305,
		{KeyCode::F17, "F17"}, //306,
		{KeyCode::F18, "F18"}, //307,
		{KeyCode::F19, "F19"}, //308,
		{KeyCode::F20, "F20"}, //309,
		{KeyCode::F21, "F21"}, //310,
		{KeyCode::F22, "F22"}, //311,
		{KeyCode::F23, "F23"}, //312,
		{KeyCode::F24, "F24"}, //313,
		{KeyCode::F25, "F25"}, //314,
		{},{},{},{},{},
		{KeyCode::KP0, "KP0"}, //320,
		{KeyCode::KP1, "KP1"}, //321,
		{KeyCode::KP2, "KP2"}, //322,
		{KeyCode::KP3, "KP3"}, //323,
		{KeyCode::KP4, "KP4"}, //324,
		{KeyCode::KP5, "KP5"}, //325,
		{KeyCode::KP6, "KP6"}, //326,
		{KeyCode::KP7, "KP7"}, //327,
		{KeyCode::KP8, "KP8"}, //328,
		{KeyCode::KP9, "KP9"}, //329,
		{KeyCode::KPDecimal, "KPDecimal"}, //330,
		{KeyCode::KPDivide, "KPDivide"}, //331,
		{KeyCode::KPMultiply, "KPMultiply"}, //332,
		{KeyCode::KPSubtract, "KPSubtract"}, //333,
		{KeyCode::KPAdd, "KPAdd"}, //334,
		{KeyCode::KPEnter, "KPEnter"}, //335,
		{KeyCode::KPEqual, "KPEqual"}, //336,
		{}, {}, {},
		{KeyCode::LeftShift, "LeftShift"}, //340,
		{KeyCode::LeftControl, "LeftControl"}, //341,
		{KeyCode::LeftAlt, "LeftAlt"}, //342,
		{KeyCode::LeftSuper, "LeftSuper"}, //343,
		{KeyCode::RightShift, "RightShift"}, //344,
		{KeyCode::RightControl, "RightControl"}, //345,
		{KeyCode::RightAlt, "RightAlt"}, //346,
		{KeyCode::RightSuper, "RightSuper"}, //347,
		{KeyCode::Menu, "Menu"} //348
	};

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	const std::string& keycode_to_string(KeyCode keycode);

	KeyCode string_to_keycode(std::string& text);

}
