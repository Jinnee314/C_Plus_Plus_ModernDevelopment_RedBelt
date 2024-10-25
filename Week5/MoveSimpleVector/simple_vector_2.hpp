#include <cstdint>
#include <algorithm>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
	SimpleVector() = default;
	explicit SimpleVector(size_t size) : size(size), capacity(size)
	{
		data = new T[size];
	}
	~SimpleVector()
	{
		delete[] data;
	}

	T& operator[](size_t index) { return data[index]; }

	T* begin() { return data; }
	T* end() { return data + size; }

	const T* begin() const { return data; }
	const T* end() const { return data + size; }

	size_t Size() const { return size; }
	size_t Capacity() const { return capacity; }
	void PushBack(T value)
	{
		if (size >= capacity)
		{
			capacity = capacity == 0 ? 1 : 2 * capacity;
			auto n = new T[capacity];
			move(begin(), end(), n);
			
			delete[] data;

			data = n;
			data[size++] = move(value);
		}
		else
		{
			data[size++] = move(value);
		}
	}

	// При необходимости перегрузите
	// существующие публичные методы

private:
	// Добавьте сюда поля
	size_t size;
	size_t capacity;
	T* data;
};

//template <typename T>
//class SimpleVector {
//public:
//	SimpleVector() = default;
//	explicit SimpleVector(size_t size);
//	~SimpleVector();
//
//	T& operator[](size_t index);
//
//	T* begin();
//	T* end();
//
//	size_t Size() const;
//	size_t Capacity() const;
//
//	// принимаем объект по значению
//	void PushBack(T value);
//
//private:
//	T* data = nullptr;
//	size_t size = 0;
//	size_t capacity = 0;
//
//	void ExpandIfNeeded();
//};
//
//template <typename T>
//SimpleVector<T>::SimpleVector(size_t size)
//	: data(new T[size])
//	, size(size)
//	, capacity(size)
//{
//}
//
//template <typename T>
//SimpleVector<T>::~SimpleVector() {
//	delete[] data;
//}
//
//template <typename T>
//T& SimpleVector<T>::operator[](size_t index) {
//	return data[index];
//}
//
//template <typename T>
//size_t SimpleVector<T>::Size() const {
//	return size;
//}
//
//template <typename T>
//size_t SimpleVector<T>::Capacity() const {
//	return capacity;
//}
//
//template <typename T>
//void SimpleVector<T>::PushBack(T value) {
//	ExpandIfNeeded();
//	data[size++] = move(value);
//}
//
//template <typename T>
//void SimpleVector<T>::ExpandIfNeeded() {
//	if (size >= capacity) {
//		auto new_cap = capacity == 0 ? 1 : 2 * capacity;
//		auto new_data = new T[new_cap];
//		// используем перемещение
//		move(begin(), end(), new_data);
//		delete[] data;
//		data = new_data;
//		capacity = new_cap;
//	}
//}
//
//template <typename T>
//T* SimpleVector<T>::begin() {
//	return data;
//}
//
//template <typename T>
//T* SimpleVector<T>::end() {
//	return data + size;
//}