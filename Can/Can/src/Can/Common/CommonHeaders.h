#pragma once

#ifdef _WIN64
#pragma warning(disable: 4530) // disable exception warning
#endif // _WIN64

#include <memory>
#include <mutex>

#ifdef _WIN64
#include <DirectXMath.h>
#endif // _WIN64


#ifndef DISABLE_COPY
#define DISABLE_COPY(T)          \
explicit T(const T&) = delete;   \
T & operator=(const T&) = delete;
#endif // !DISABLE_COPY

#ifndef DISABLE_MOVE
#define DISABLE_MOVE(T)      \
explicit T(T&&) = delete;    \
T & operator=(T&&) = delete;
#endif // !DISABLE_MOVE

#ifndef DISABLE_COPY_AND_MOVE
#define DISABLE_COPY_AND_MOVE(T) \
DISABLE_COPY(T)                  \
DISABLE_MOVE(T)
#endif // !DISABLE_COPY_AND_MOVE

#if _DEBUG
#define DEBUG_OP(op) op
#else 
#define DEBUG_OP(op)
#endif

#include "Can/Common/PrimitiveTypes.h"
#include "Can/Utilities/Math.h"
#include "Can/Utilities/Utilities.h"
#include "Can/Utilities/MathTypes.h"
#include "Id.h"