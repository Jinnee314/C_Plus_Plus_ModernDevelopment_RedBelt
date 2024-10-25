#pragma once
#include <algorithm>

template <typename T>
class SimpleVector {
public:
	SimpleVector() : size(0), capacity(0), data(nullptr) {}

	explicit SimpleVector(size_t size) : size(size), capacity(size)
	{
		data = new T[size];
	}
	explicit SimpleVector(const SimpleVector& sv) : data(new T[sv.Capacity()]), size(sv.Size()), capacity(sv.Capacity()) 
	{
		std::copy(sv.begin(), sv.end(), begin());
	}

	~SimpleVector()
	{
		delete[] data;
	}

	void operator=(const SimpleVector& sv)
	{
		/*size_t i = 0;

		for (i; i < size && i < sv.Size(); ++i)
		{
			data[i] = sv[i];
		}

		if (size == sv.Size())
		{
			return *this;
		}
		
		if (size < sv.Size())
		{
			for (i = 0; i < sv.Size(); ++i)
			{
				this->PushBack(sv[i]);
			}
		}
		else
		{
			size = sv.Size();
		}

		return *this;*/
		if (sv.size <= capacity)
		{
			std::copy(sv.begin(), sv.end(), begin());
		}
		else
		{
			// Это так называемая идиома copy-and-swap.
			// Мы создаём временный вектор с помощью
			// конструктора копирования, а затем обмениваем его поля со своими.
			// Так мы достигаем двух целей:
			//  - избегаем дублирования кода в конструкторе копирования
			//    и операторе присваивания
			//  - обеспечиваем согласованное поведение конструктора копирования
			//    и оператора присваивания
			SimpleVector<T> tmp(sv);
			std::swap(tmp.data, data);
			std::swap(tmp.size, size);
			std::swap(tmp.capacity, capacity);
		}
	}

	T& operator[](size_t index) 
	{
		return data[index];
	}
	
	const T& operator[](size_t index) const
	{
		return data[index];
	}

	T* begin() { return data; }
	T* end() { return data + size; }

	const T* begin() const { return data; }
	const T* end() const { return data + size; }

	size_t Size() const { return size; }

	size_t Capacity() const { return capacity; }

	void PushBack(const T& value)
	{
		if (size >= capacity)
		{
			capacity = capacity == 0 ? 1 : 2 * capacity;
			T* newp = new T[capacity];

			std::copy(begin(), end(), newp);

			delete[] data;
			data = newp;
		}
		data[size++] = value;
	}

private:
	size_t size;
	size_t capacity;
	T* data;
};

