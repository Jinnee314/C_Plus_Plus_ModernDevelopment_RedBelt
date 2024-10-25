#pragma once

#include <stdexcept>
#include <array>

using namespace std;

template <typename T, size_t N>
class StackVector {
public:
	explicit StackVector(size_t a_size = 0) : size(a_size)	
	{
		if (size > N)
		{
			throw invalid_argument("Size > Capacity.");
		}
	}

	T& operator[](size_t index) { return arr[index]; }
	const T& operator[](size_t index) const { return arr[index]; }


	std::_Array_iterator<T,N> begin() { return arr.begin(); }
	std::_Array_iterator<T, N> end() { return arr.begin() + size; }
	const std::_Array_iterator<T, N> begin() const { return arr.begin(); }
	const std::_Array_iterator<T, N> end() const { return arr.begin() + size; }

	size_t Size() const { return size; }
	size_t Capacity() const { return N; }

	void PushBack(const T& value)
	{
		if (size >= N)
		{
			throw overflow_error("Øt is impossible to add an object: the vector is full.");
		}
		arr[size] = value;
		++size;
	}
	T PopBack()
	{
		if (size == 0)
		{
			throw underflow_error("Vector empty");
		}
		--size;
		return arr[size];
	}

private:
	size_t size;
	array<T, N> arr;

};

