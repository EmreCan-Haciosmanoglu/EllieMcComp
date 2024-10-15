#pragma once
#include "Can\Common\CommonHeaders.h"
#include "Can\Core.h"
#include  <type_traits>

namespace Can::utl
{
	class blob_stream_reader
	{
	public:
		DISABLE_COPY_AND_MOVE(blob_stream_reader);
		explicit blob_stream_reader(const u8* buffer)
			: _buffer{ buffer }
			, _position{ buffer }
		{
			assert(buffer);
		}

		template<typename T>
		T read()
		{
			static_assert(std::is_arithmetic_v<T>, "Template argument should be a primite type.");
			T value{ *((T*)_position) };
			_position += sizeof(T);
			return value;
		}

		void read(u8* buffer, u64 length)
		{
			memcpy(buffer, _position, length);
			_position += length;
		}

		void skip(u64 offset)
		{
			_position += offset;
		}

		[[nodiscard]] constexpr const u8* const buffer_start() const { return _buffer; }
		[[nodiscard]] constexpr const u8* const position() const { return _position; }
		[[nodiscard]] constexpr u64 offset() const { return _position - _buffer; }

	private:
		const u8* const _buffer{ nullptr };
		const u8* _position{ nullptr };
	};

	class blob_stream_writer
	{
	public:
		DISABLE_COPY_AND_MOVE(blob_stream_writer);
		explicit blob_stream_writer(u8* buffer, u64 buffer_size)
			: _buffer{ buffer }
			, _position{ buffer }
			, _buffer_size{ buffer_size }
		{
			assert(buffer && buffer_size);
		}

		template<typename T>
		void write(T value)
		{
			static_assert(std::is_arithmetic_v<T>, "Template argument should be a primite type.");
			assert(&_position[sizeof(T)] <= &_buffer[_buffer_size]);
			*((T*)_position) = value;
			_position += sizeof(T);
		}

		void write(const char* buffer, u64 length)
		{
			assert(&_position[length] <= &_buffer[_buffer_size]);
			memcpy(_position, buffer, length);
			_position += length;
		}

		void write(const u8* buffer, u64 length)
		{
			assert(&_position[length] <= &_buffer[_buffer_size]);
			memcpy(_position, buffer, length);
			_position += length;
		}

		void skip(u64 offset)
		{
			assert(&_position[offset] <= &_buffer[_buffer_size]);
			_position += offset;
		}

		[[nodiscard]] constexpr const u8* const buffer_start() const { return _buffer; }
		[[nodiscard]] constexpr const u8* const buffer_end() const { return &_buffer[_buffer_size]; }

	private:
		u8* _buffer{ nullptr };
		u8* _position{ nullptr };
		u64 _buffer_size{ 0 };
	};
}