/**
 * \file   leela/Object.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Header file for the Object class.
 */

#ifndef OBJECT_H_408JFDVEW32
#define OBJECT_H_408JFDVEW32

#include <cstddef>

#define REF(var, expr) Ref<typeof(expr)> var = (expr)
#define SENTINEL 1337

class Object {
private:
	struct RingItem {
	public:
		RingItem* previous;
		RingItem* next;
		
		Object*   obj;
		
		RingItem()
		{
			previous = NULL;
			next = NULL;
			obj = NULL;
		}
		
		~RingItem()
		{
			if (previous)
				previous->next = next;
			if (next)
				next->previous = previous;
			previous = NULL;
			next = NULL;
			obj = NULL;
		}
		
		void insert(RingItem** ring)
		{
			if (*ring == NULL) {
				*ring = this;
				return;
			}
			
			next = (*ring)->next;
			previous = *ring;
			if ((*ring)->next)
				(*ring)->next->previous = this;
			(*ring)->next = this;
		}
		
		void dispose()
		{
			delete obj;
			obj = NULL;
		}
	};
	
	static RingItem* _ring;

	int _refCount;
	bool _alive;
	int _sentinel;

public:
	Object();
	virtual ~Object();

	void claim();
	void checkHealth() const;
	static Object* release(Object* obj);
	
	static void deleteAll();
};

template<class T>
class Ref {
private:
	T* _ptr;

	void setPtr(T* value)
	{
		if (_ptr == value)
			return;
		
		if (_ptr != NULL)
			Object::release(_ptr);
		_ptr = value;
		if (_ptr != NULL)
			_ptr->claim();
	}

public:
	Ref()
	{
		_ptr = NULL;
		setPtr(NULL);
	}
	
	Ref(T* ptr)
	{
		_ptr = NULL;
		setPtr(ptr);
	}
	
	Ref(const Ref<T>& ref)
	{
		_ptr = NULL;
		setPtr(ref.getPtr());
	}
	
	~Ref()
	{
		setPtr(NULL);
	}
	
	T& operator*()
	{
		return *_ptr;
	}
	
	T* operator->() const
	{
		return _ptr;
	}
	
	template<class U>
	operator Ref<U> () const
	{
		// Ref<U> r(dynamic_cast<U*>(getPtr()));
		Ref<U> r((U*) getPtr());
		return r;
	}
	
	Ref<T>& operator=(T* ptr)
	{
		setPtr(ptr);
		return *this;
	}
	
	Ref<T>& operator=(const Ref<T>& other)
	{
		if (this == &other) return *this;
		
		setPtr(other.getPtr());
		return *this;
	}
	
	/*
	template<class U>
	Ref<T>& operator=(const Ref<U>& other)
	{
		if (this == &other) return *this;
		
		// setPtr(dynamic_cast<T*>(other.getPtr()));
		setPtr((T*) other.getPtr());
		return *this;
	}
	*/
	
	bool isNull() const { return _ptr == NULL; }
	bool isNotNull() const { return !isNull(); }
	
	T* getPtr() const
	{
		if (_ptr != NULL) _ptr->checkHealth();
		return _ptr;
	}
};

#endif /* end of include guard: OBJECT_H_408JFDVEW32 */

