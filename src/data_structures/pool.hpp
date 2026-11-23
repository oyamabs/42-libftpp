#pragma once

#include <memory>
#include <vector>
template <typename T>
class Pool {
private:
	std::vector<std::unique_ptr<T>>	allocated;
	std::vector<T*>					available;
	size_t							total = 0;

	void	grow(const size_t &newSize);
	void	shrink(const size_t &newSize);

	void	returnObject(T* obj);
public:
	void	resize(const size_t& number);

	class Object {
	private:
		Pool<T>*	pool;
		T*			object;

		Object(Pool<T>* pool, T* object);

		friend class Pool;
	public:
		~Object();
		T* operator->();
	};

	template <typename... TArgs>
	Object acquire(TArgs&&... p_args);
};
