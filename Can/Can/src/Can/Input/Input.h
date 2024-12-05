#pragma once
#include "Can/Common/CommonHeaders.h"
#include "Can/API/Input.h"

namespace Can::input
{
	void set(input_source::type type, input_code::code code, math::v3 value);
}