/*
The MIT License (MIT)

Copyright (c) 2015 Scott Englert

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef MAYAITERATION_MAYA_ARRAY_RANGE_H_
#define MAYAITERATION_MAYA_ARRAY_RANGE_H_

#include <iterator>

namespace mayaiteration {

/**
Maya Array Range Class Template

DESCRIPTION:
This template class is intended to provide Maya array classes a std iterator
interface. This makes working with Maya's arrays more friendly and consistent
with the C++ standard library. This allows use of iterator features with out
sacrificing performance of copying the array to and from other containers. This
template class should work for all of Maya's internal *Array classes that have
an "[]"" operator and a "length" member function.

A MayaArrayRange instance provides the commonly used "begin()" and "end()"
functions. These return either an iterator or a const iterator. See the
std iterator documentation for more information on using the iterator instance.

USAGE:
	MPointArray myPointArray(5); // array of 5 points
	mayaiteration::MayaArrayRange<MPointArray> myPointArrayRange(myPointArray);
	
	// create iterators for the beginning and end of the array
	mayaiteration::MayaArrayRange<MPointArray>::iterator beginIt = myPointArrayRange.begin();
	mayaiteration::MayaArrayRange<MPointArray>::iterator endIt = myPointArrayRange.end();

	// Assign a new point to the first point in the array
	*beginIt = MPoint(1.0, 1.0, 1.0);
	
	// dereferencing to get the x value of the point
	std::cout << "X= " << beginIt->x << std::endl;

	// using regular loops
	for(auto it = beginIt; it != endIt; ++it)
	   std::cout << *pnt << std::endl;

	// using new range based loop in C++11
	for(auto pnt : myPointArrayRange)
	    std::cout << pnt << std::endl;

	// sort the array using a comparsion function
	std::sort(beginIt, endIt, myPointCompareFunc);

	// getting the number of points between two iterators
	mayaiteration::MayaArrayRange<MPointArray>::iterator::difference_type dist = std::distance(beginIt, endIt);
	std::cout << dist << std::endl;
*/
template<typename T>
class MayaArrayRange {
protected:
	typedef typename std::remove_reference<decltype(std::declval<T>()[0])>::type item_type;

public:
	template<typename C, typename V>
	class MayaArrayIter : public std::iterator<std::random_access_iterator_tag, V, int>	{
	friend class MayaArrayRange;

	protected:
		C* c;
		unsigned int i;

		MayaArrayIter(C* c) : c(c), i(0) {}
		MayaArrayIter(C* c, unsigned int i) : c(c), i(i) {}

	public:
		typedef typename std::iterator<std::random_access_iterator_tag, V, int>::pointer pointer;
		typedef typename std::iterator<std::random_access_iterator_tag, V, int>::reference reference;
		typedef typename std::iterator<std::random_access_iterator_tag, V, int>::difference_type difference_type;

		template<typename C2, typename V2>
		MayaArrayIter(const MayaArrayIter<C2, V2>& other) : c(other.c), i(other.i) {}
		
		template<typename C2, typename V2>
		MayaArrayIter& operator=(const MayaArrayIter<C2, V2>& other) {
			c = other.c;
			i = other.i;
			return *this;
		}

		reference operator*() const	{
			return (*c)[i];
		}

		pointer operator->() const {
			return &(*c)[i];
		}

		MayaArrayIter& operator++()	{
			++i;
			return *this;
		}

		MayaArrayIter& operator--()	{
			--i;
			return *this;
		}

		MayaArrayIter operator++(int) {
			return MayaArrayIter(c, i++);
		}

		MayaArrayIter operator--(int) {
			return MayaArrayIter(c, i--);
		}

		MayaArrayIter operator+(const difference_type& n) const	{
			return MayaArrayIter(c, (i + n));
		}

		MayaArrayIter& operator+=(const difference_type& n)	{
			i += n;
			return *this;
		}

		MayaArrayIter operator-(const difference_type& n) const	{
			return MayaArrayIter(c, (i - n));
		}

		MayaArrayIter& operator-=(const difference_type& n)	{
			i -= n;
			return *this;
		}

		reference operator[](const difference_type& n) const {
			return (*c)[i + n];
		}

		bool operator==(const MayaArrayIter& other) const {
			return i == other.i;
		}

		bool operator!=(const MayaArrayIter& other) const {
			return i != other.i;
		}

		bool operator<(const MayaArrayIter& other) const {
			return i < other.i;
		}

		bool operator>(const MayaArrayIter& other) const {
			return i > other.i;
		}

		bool operator<=(const MayaArrayIter& other) const {
			return i <= other.i;
		}

		bool operator>=(const MayaArrayIter& other) const {
			return i >= other.i;
		}

		difference_type operator+(const MayaArrayIter& other) const	{
			return i + other.i;
		}

		difference_type operator-(const MayaArrayIter& other) const	{
			return i - other.i;
		}
	};

	typedef MayaArrayIter<T, item_type> iterator;
	typedef MayaArrayIter<const T, const item_type> const_iterator;
	
	MayaArrayRange(T* mayaArray) : a(mayaArray) {}
	MayaArrayRange(T& mayaArray) : a(&mayaArray) {}
	
	iterator begin() {
		return iterator(a);
	}

	const_iterator begin() const {
		return const_iterator(a);
	}

	const_iterator cbegin() const {
		return const_iterator(a);
	}

	iterator end() {
		return iterator(a, a->length());
	}

	const_iterator end() const {
		return const_iterator(a, a->length());
	}

	const_iterator cend() const {
		return const_iterator(a, a->length());
	}

protected:
	T* a;
};

} // namespace mayaiteration

#endif // MAYAITERATION_MAYA_ARRAY_RANGE_H_