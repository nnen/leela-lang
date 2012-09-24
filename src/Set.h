/**
 * \file   Set.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-04-27
 *
 * \brief  
 */

#ifndef SET_H_203D44GFGB2321XD4G3DX5G
#define SET_H_203D44GFGB2321XD4G3DX5G

#include <set>

using namespace std;

template<class T>
class Set : public set<T> {
public:
	Set() : set<T>() {}
	
	Set(T item) : set<T>()
	{
		this->insert(item);
	}
	
	virtual ~Set() {}
	
	bool contains(T value) { return this->count(value) > 0; }
	
	void merge(const Set<T>& other)
	{
		foreach (item, other)
			this->insert(*item);
	}
	
	void intersect(const Set<T>& other, Set<T>& result)
	{
		foreach (item, other)
			if (contains(*item)) result.insert(*item);
	}
	
	Set<T> intersect(const Set<T>& other)
	{
		Set<T> result;
		intersect(other, result);
		return result;
	}
};

template<class T>
class RefSet : public Set<Ref<T> > {
public:
	RefSet() : Set<Ref<T> >() {}
	virtual ~RefSet() {}
};

#endif /* end of include guard: SET_H_203D44GFGB2321XD4G3DX5G */

