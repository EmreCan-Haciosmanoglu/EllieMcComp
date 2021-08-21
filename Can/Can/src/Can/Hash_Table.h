#pragma once

#include "Can/Core.h"
#include <array>

namespace Can
{
	template<typename Value>
	struct Linked_List_Item
	{
		u64 key = 0;
		Value* item = nullptr;
		Linked_List_Item<Value>* next_item = nullptr;
	};

	template<typename Value, u64 N>
	struct Hash_Table
	{
		std::array<Linked_List_Item<Value>*, N> table;
	};


	template< typename Value, u64 N>
	void insert_or_replace(Hash_Table<Value, N>& hash_table, u64 key, Value* value)
	{
		bool exist = false;
		u64 index = key % N;
		Linked_List_Item<Value, N>* prev_item = nullptr;
		Linked_List_Item<Value, N>* item = hash_table.table[index];
		while (item)
		{
			if (item->key == key)
			{
				exist = true;
				delete item->item;
				item->item = value;
				break;
			}
			prev_item = item;
			item = item->next_item;
		}
		if (!exist)
		{
			Linked_List_Item* new_item = new Linked_List_Item<Value, N>();
			new_item->key = key;
			new_item->item = value;
			if (prev_item)
				prev_item->next_item = new_item;
			else
				hash_table.table[index] = new_item;
		}
	}

	template< typename Value, u64 N>
	Value* get_or_insert(Hash_Table<Value, N>& hash_table, u64 key, Value* value)
	{
		u64 index = key % N;
		Linked_List_Item<Value, N>* prev_item = nullptr;
		Linked_List_Item<Value, N>* item = hash_table.table[index];
		while (item)
		{
			if (item->key == key)
				return item->item;

			prev_item = item;
			item = item->next_item;
		}
		Linked_List_Item* new_item = new Linked_List_Item<Value, N>();
		new_item->key = key;
		new_item->item = value;
		if (prev_item)
			prev_item->next_item = new_item;
		else
			hash_table.table[index] = new_item;
		return value;
	}

	template< typename Value, u64 N>
	Value* get_or_init(Hash_Table<Value, N>& hash_table, u64 key, bool& first_time)
	{
		u64 index = key % N;
		Linked_List_Item<Value>* prev_item = nullptr;
		Linked_List_Item<Value>* item = hash_table.table[index];
		while (item)
		{
			if (item->key == key)
			{
				first_time = false;
				return item->item;
			}

			prev_item = item;
			item = item->next_item;
		}
		Value* value = new Value();

		Linked_List_Item<Value>* new_item = new Linked_List_Item<Value>();
		new_item->key = key;
		new_item->item = value;
		if (prev_item)
			prev_item->next_item = new_item;
		else
			hash_table.table[index] = new_item;
		first_time = true;
		return value;
	}
	template< typename Value, u64 N>
	Value* get_or_init(Hash_Table<Value, N>& hash_table, u64 key)
	{
		u64 index = key % N;
		Linked_List_Item<Value>* prev_item = nullptr;
		Linked_List_Item<Value>* item = hash_table.table[index];
		while (item)
		{
			if (item->key == key)
				return item->item;

			prev_item = item;
			item = item->next_item;
		}
		Value* value = new Value();

		Linked_List_Item<Value>* new_item = new Linked_List_Item<Value>();
		new_item->key = key;
		new_item->item = value;
		if (prev_item)
			prev_item->next_item = new_item;
		else
			hash_table.table[index] = new_item;
		return value;
	}
}