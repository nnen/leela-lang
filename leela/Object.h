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
#include <exception>

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

class NullReferenceException : public std::exception {
public:
	NullReferenceException() throw() : std::exception() {}
	NullReferenceException(const NullReferenceException& e) throw() : std::exception() {}
	NullReferenceException& operator= (const NullReferenceException& e) throw() { return *this; }
	virtual ~NullReferenceException() throw() {}

	virtual const char* what() const throw() { return "Attempted to access null reference."; }
};

class InvalidCastException : public std::exception {
public:
	InvalidCastException() throw() : std::exception() {}
	InvalidCastException(const InvalidCastException& e) throw() : std::exception() {}
	InvalidCastException& operator= (const InvalidCastException& e) throw() { return *this; }
	virtual ~InvalidCastException() throw() {}
	
	virtual const char* what() const throw() { return "Invalid dynamic cast."; }
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
		if (isNull())
			throw NullReferenceException();
		return *getPtr();
	}
	
	T* operator->() const
	{
		if (isNull())
			throw NullReferenceException();
		return getPtr();
	}
	
	template<class U>
	operator Ref<U> () const
	{
		// Ref<U> r(dynamic_cast<U*>(getPtr()));
		Ref<U> ref = this->as<U>();
		if (ref.isNull())
			throw InvalidCastException();
		return ref;
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

	template<class U>
	Ref<U> as() const
	{
		return Ref<U>(dynamic_cast<U*>(getPtr()));
	}
};

#endif /* end of include guard: OBJECT_H_408JFDVEW32 */

