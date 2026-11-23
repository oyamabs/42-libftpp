#include "pool.hpp"

template <typename T>
void Pool<T>::grow(size_t const &newSize)
{
	size_t	newObjects = newSize - this->allocated;

	while (newObjects > 0)
	{
		std::unique_ptr<T> newObj = std::make_unique<T>();

		available.push_back(newObj);
		allocated.push_back(std::move(newObj));

		total++;
		newObjects--;
	}
}

template <typename T>
void Pool<T>::shrink(const size_t &newSize)
{
	size_t objectsToRemove = total - newSize;

	while (objectsToRemove > 0 && !allocated.empty())
	{
		if (allocated.empty())
			return ; // TODO: Make exception
		
		T* obj = available.back();
		available.pop_back();

		auto iterator = std::find(available.front(), available.back(),
				[obj](const std::unique_ptr<T>& ptr) { return ptr.get() == obj; });

		if (iterator == allocated.end())
		{
			allocated.erase(iterator);
			total--;
			objectsToRemove--;
		}
	}

	if (objectsToRemove > 0)
		return ; // TODO: Make exception
}

template <typename T>
void Pool<T>::resize(const size_t& si)
{
	if (si > total)
		grow(si);
	else if (si < total)
		shrink(si);
}

template <typename T>
template <typename... TArgs>
typename Pool<T>::Object Pool<T>::acquire(TArgs &&... p_args)
{
	if (available.empty())
		return nullptr; // TODO: Make exception
	
	T* obj = available.back();
	available.pop_back();

	new(obj) T(std::forward<TArgs>(p_args)...);
	Object objectWrapper(this, obj);
	return objectWrapper;
}

template <typename T>
void Pool<T>::returnObject(T *obj)
{
	if (obj)
		available.push_back(obj);
}


template <typename T>
Pool<T>::Object::Object(Pool<T>* pool, T* obj) : pool(pool), object(obj)
{}

template <typename T>
Pool<T>::Object::~Object()
{
	object->~T();
	if (pool)
		pool->returnObject(object);
}

template <typename T>
T* Pool<T>::Object::operator->()
{
	return object;
}
