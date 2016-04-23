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

#ifndef MAYAARRAY_MAYA_ARRAY_H_
#define MAYAARRAY_MAYA_ARRAY_H_

#include <cassert>
#include <iterator>

namespace mayaarray {

/**
Generic Maya Array Class Template

DESCRIPTION:
This class template is intended to be more generic and compatible with the C++
standard library and algorithms. One example is to use this class for creating
iterators so you do not have to copy data to another standard library
container, which saves memory and performance. This class should work for all
of Maya's internal *Array classes that have the "[]"" operator and a "length"
member function.

A MayaArray instance provides the commonly used "begin()" and "end()"
functions. This returns a random access compliant iterator. See the
standard library iterator documentation for more information on using
the iterator instance. More functionality will be added for more support.

USAGE:
	MPointArray myPointArray(5); // array of 5 points
	mayaarray::MayaArray<MPointArray> myArray(myPointArray);
	
	// create iterators for the beginning and end of the array
	mayaarray::MayaArray<MPointArray>::iterator beginIt = myArray.begin();
	mayaarray::MayaArray<MPointArray>::iterator endIt = myArray.end();

	// Assign a new point to the first point in the array
	*beginIt = MPoint(1.0, 1.0, 1.0);
	
	// dereferencing to get the x value of the point
	std::cout << "X= " << beginIt->x << std::endl;

	// using regular loops
	for(auto it = beginIt; it != endIt; ++it)
	   std::cout << *it << std::endl;

	// using new range based loop in C++11
	for(auto pnt : myArray)
	    std::cout << pnt << std::endl;

	// sort the array using a comparsion function
	std::sort(beginIt, endIt, myPointCompareFunc);

	// getting the number of points between two iterators
	mayaarray::MayaArray<MPointArray>::iterator::difference_type dist = std::distance(beginIt, endIt);
	std::cout << dist << std::endl;

	// append 3 MPoints to the array
	std::fill_n(std::back_inserter(myArray), 3, MPoint());
*/
template<typename T>
class MayaArray {
protected:
	T& mArray;

public:
	template<typename C, typename V>
	class MayaArrayIter : public std::iterator<std::random_access_iterator_tag, V, int>	{
	friend class MayaArray;

	protected:
		C& c;
		unsigned int i;

		MayaArrayIter(C& c) : c(c), i(0) {}
		MayaArrayIter(C& c, unsigned int i) : c(c), i(i) {}

	public:
		template<typename C2, typename V2>
		MayaArrayIter(const MayaArrayIter<C2, V2>& other) : c(other.c), i(other.i) {}
		
		template<typename C2, typename V2>
		MayaArrayIter& operator=(const MayaArrayIter<C2, V2>& other) {
			c = other.c;
			i = other.i;
			return *this;
		}

		reference operator*() const	{
			assert(i < c.length());
			return c[i];
		}

		pointer operator->() const {
			assert(i < c.length());
			return &c[i];
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
			return c[i + n];
		}

		bool operator==(const MayaArrayIter& other) const {
			return (i == other.i && &c == &other.c);
		}

		bool operator!=(const MayaArrayIter& other) const {
			return !(*this == other);
		}

		bool operator<(const MayaArrayIter& other) const {
			return (i < other.i && &c == &other.c);
		}

		bool operator>(const MayaArrayIter& other) const {
			return !(*this < other || *this == other);
		}

		bool operator<=(const MayaArrayIter& other) const {
			return !(*this > other);
		}

		bool operator>=(const MayaArrayIter& other) const {
			return !(*this < other);
		}

		difference_type operator+(const MayaArrayIter& other) const	{
			return i + other.i;
		}

		difference_type operator-(const MayaArrayIter& other) const	{
			return i - other.i;
		}
	};

	typedef typename std::remove_reference<decltype(std::declval<T>()[0])>::type value_type;
	typedef const value_type const_reference;
	typedef value_type reference;
	typedef unsigned int size_type;

	typedef MayaArrayIter<T, value_type> iterator;
	typedef MayaArrayIter<const T, const value_type> const_iterator;

	MayaArray(T* mayaArray) : mArray(*mayaArray) {}
	MayaArray(T& mayaArray) : mArray(mayaArray) {}
	
	// iteration member functions
	iterator begin() {
		return iterator(mArray);
	}

	const_iterator begin() const {
		return const_iterator(mArray);
	}

	const_iterator cbegin() const {
		return const_iterator(mArray);
	}

	iterator end() {
		return iterator(mArray, mArray.length());
	}

	const_iterator end() const {
		return const_iterator(mArray, mArray.length());
	}

	const_iterator cend() const {
		return const_iterator(mArray, mArray.length());
	}

	// modifier member functions
	void push_back(const value_type& value) {
		mArray.append(value);
	}

	void push_front(const value_type& value) {
		mArray.insert(value, 0);
	}

	reference operator[](size_type pos) {
		return mArray[pos];
	}

	const_reference operator[](size_type pos) const {
		return mArray[pos];
	}

};

} // namespace mayaarray

#endif // MAYAARRAY_MAYA_ARRAY_H_