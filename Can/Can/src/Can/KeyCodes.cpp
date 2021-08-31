#include "canpch.h"
#include "KeyCodes.h"

namespace Can
{
	const std::string& keycode_to_string(KeyCode keycode)
	{
		s16 val = (u16)keycode - 32;
		assert(val >= 0 && val < 317);
		
		const String_Keycode_Pair& string_keycode_pair = strings[val];
		assert(string_keycode_pair.string != "");

		return string_keycode_pair.string;
	}

	KeyCode string_to_keycode(std::string& text)
	{
		for (u64 i = 0; i < 26; i++)
			if (text.compare(strings[i].string))
				return strings[i].keycode;

		return strings[0].keycode;
	}

}