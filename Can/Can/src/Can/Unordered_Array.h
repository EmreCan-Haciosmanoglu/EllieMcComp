#pragma once
#include "Can.h"

namespace Can
{
	template<typename T>
	class Unordered_Array_Element
	{
	public:
		Unordered_Array_Element() : value{} {}

		template<typename T, typename... Ts>
		Unordered_Array_Element(Ts... args) : value{ args... } {}
		T value;
		bool valid = false;
	};

	constexpr u64 array_start_size{ 4 };

	template<typename T>
	class Unordered_Array
	{
	public:
		Unordered_Array(u64 c = array_start_size)
		{
			assert(c >= array_start_size);
			capacity = c;
			values = new Unordered_Array_Element<T>[c];
			for (u64 i{ 0 }; i < c; ++i)
			{
				values[i].valid = false;
				T::reset_to_default(&values[i].value);
			}
		}

		~Unordered_Array()
		{
			assert(values);
			delete[] values;
		}

		u64 size{ 0 };
		u64 capacity{ 0 };
		Unordered_Array_Element<T>* values{ nullptr };

		[[nodiscard]] T& operator[] (u64 x) {
			return values[x].value;
		}

		[[nodiscard]] constexpr bool is_valid(u64 index) const { assert(index < capacity); return values[index].valid; }
	};

	template<typename T>
	void new_unordered_array(
		Unordered_Array<T>* unordered_array,
		u64 capacity
	) {
		assert(unordered_array->size <= unordered_array->capacity);
		unordered_array->size = 0;
		unordered_array->capacity = capacity;
		unordered_array->values = new Unordered_Array_Element<T>[capacity];
	}

	template<typename T>
	std::pair<T*, u64> array_add(
		Unordered_Array<T>* unordered_array,
		T* value
	)
	{
		if (unordered_array->size < unordered_array->capacity)
		{
			u64 empty_index = 0;
			for (; empty_index < unordered_array->size; empty_index++)
				if (!unordered_array->values[empty_index].valid)
					break;
			unordered_array->values[empty_index].valid = true;
			T::move(&unordered_array->values[empty_index].value, value);
			unordered_array->size++;
			return std::make_pair(&unordered_array->values[empty_index].value, empty_index);
		}
		else
		{
			array_resize(unordered_array, (u64)((f32)unordered_array->capacity * 1.5f));

			unordered_array->values[unordered_array->size].valid = true;
			T::move(&unordered_array->values[unordered_array->size].value, value);
			unordered_array->size++;
			return std::make_pair(&unordered_array->values[unordered_array->size - 1].value, unordered_array->size - 1);
		}
	}

	template<typename T, typename... Args>
	std::pair<T*, u64> array_emplace(
		Unordered_Array<T>* unordered_array,
		Args&&... args
	)
	{
		if (unordered_array->size < unordered_array->capacity)
		{
			u64 empty_index = 0;
			for (; empty_index < unordered_array->size; empty_index++)
				if (!unordered_array->values[empty_index].valid)
					break;
			unordered_array->values[empty_index].valid = true;
			new (&unordered_array->values[empty_index].value) T(std::forward<Args>(args)...);
			unordered_array->size++;
			return std::make_pair(&unordered_array->values[empty_index].value, empty_index);
		}
		else
		{
			array_resize(unordered_array, (u64)((f32)unordered_array->capacity * 1.5f));

			unordered_array->values[unordered_array->size].valid = true;
			new (&unordered_array->values[unordered_array->size].value) T(std::forward<Args>(args)...);
			unordered_array->size++;
			return std::make_pair(&unordered_array->values[unordered_array->size - 1].value, unordered_array->size - 1);
		}
	}

	template<typename T>
	std::pair<T*, u64> array_add_empty(
		Unordered_Array<T>* unordered_array
	)
	{
		if (unordered_array->size < unordered_array->capacity)
		{
			u64 empty_index = 0;
			for (; empty_index < unordered_array->size; empty_index++)
				if (!unordered_array->values[empty_index].valid)
					break;
			unordered_array->values[empty_index].valid = true;
			unordered_array->size++;
			return std::make_pair(&unordered_array->values[empty_index].value, empty_index);
		}
		else
		{
			array_resize(unordered_array, (u64)((f32)unordered_array->capacity * 1.5f));

			unordered_array->values[unordered_array->size].valid = true;
			T::reset_to_default(&unordered_array->values[unordered_array->size].value);
			unordered_array->size++;
			return std::make_pair(&unordered_array->values[unordered_array->size - 1].value, unordered_array->size - 1);
		}
	}

	template<typename T>
	void array_remove(
		Unordered_Array<T>* unordered_array,
		u64 index
	)
	{
		assert(index >= 0);
		assert(index < unordered_array->capacity);
		assert(unordered_array->values[index].valid);

		unordered_array->values[index].valid = false;
		unordered_array->size--;

		T::remove(&unordered_array->values[index].value);
	}

	template<typename T>
	void array_resize(
		Unordered_Array<T>* unordered_array,
		u64 size
	)
	{
		assert(size >= 0);
		if (size <= unordered_array->size)
			return;

		unordered_array->capacity = size;
		auto* new_values = new Unordered_Array_Element<T>[unordered_array->capacity];
		for (u64 i = 0; i < unordered_array->size; i++)
		{
			new_values[i].valid = true;
			T::move(&new_values[i].value, &unordered_array->values[i].value);
		}
		for (u64 i = unordered_array->size; i < unordered_array->capacity; i++)
		{
			new_values[i].valid = false;
			T::reset_to_default(&new_values[i].value);
		}
		delete[] unordered_array->values;
		unordered_array->values = new_values;
	}
}