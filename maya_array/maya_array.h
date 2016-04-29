/*
The MIT License (MIT)

Copyright (c) 2016 Scott Englert

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

#include <stdexcept>
#include <maya_iteration/maya_array_range.h>

namespace mayaarray {

/**
Generic Maya Array Class Template

DESCRIPTION:
This class template container that uses a M***Array for the underlying container.
It is intended to be more generic and compatible with the C++ standard library
and algorithms, but able to be used in Maya's API to get benefits of both worlds
without copying data from one type to another. One example is to use this class
for creating iterators and using more generic programming features. This class should
work for all of Maya's internal ***Array classes. This class is mostly modeled after
the standard library vector class. More featues will be added to take advantage
of C++ features as they become available.

A MayaArray instance provides the commonly used "begin()" and "end()"
functions. This returns a random access compliant iterator. See the
standard library iterator documentation for more information on using
the iterator instance. More functionality will be added for more support.

USAGE:
	// Create an empty MPointArray that is wrapped around this generic interface
	mayaarray::MayaArray<MPointArray> meshPoints;

	// get all the points from a MFnMesh object
	myMeshFn.getPoints(meshPoints.array());

	// copy points from an existing MPointArray
	MPointArray otherPoints(5);
	mayaarray::MayaArray<MPointArray> pointsCopy(otherPoints);
	
	// create iterators for the beginning and end of the array
	mayaarray::MayaArray<MPointArray>::iterator beginIt = meshPoints.begin();
	mayaarray::MayaArray<MPointArray>::iterator endIt = meshPoints.end();

	// Assign a new point to the first point in the array
	*beginIt = MPoint(1.0, 1.0, 1.0);
	
	// dereferencing to get the x value of the point
	std::cout << "X= " << beginIt->x << std::endl;

	// using regular for loops
	for(auto it = beginIt; it != endIt; ++it)
	   std::cout << *it << std::endl;

	// using new range based loop in C++11
	for(auto pnt : meshPoints)
	    std::cout << pnt << std::endl;

	// sort the array using a comparsion function
	std::sort(beginIt, endIt, myPointCompareFunc);

	// getting the number of points between two iterators
	auto pointCount = std::distance(beginIt, endIt);
	std::cout << pointCount << std::endl;

	// append 3 default MPoints to the array using standard library algorithm fill_n
	std::fill_n(std::back_inserter(meshPoints), 3, MPoint());

	// erase points from beginIt to endIt
	meshPoints.erase(beginIt, endIt);
*/
template<typename T>
class MayaArray {
protected:
	T mArray;

public:
	// there are different types returned based if the array is const or not
	typedef typename decltype(std::declval<T>()[0]) reference;
	typedef typename decltype(std::declval<const T>()[0]) const_reference;
	typedef typename std::remove_reference<reference>::type value_type;
	typedef unsigned int size_type;

	typedef typename mayaiteration::MayaArrayRange<T>::iterator iterator;
	typedef typename mayaiteration::MayaArrayRange<T>::const_iterator const_iterator;

	/**
    Creates an empty array
	*/
	MayaArray() {}

	/**
    Creates an array of "count" elements with the given value.

	\param[in] count number of elements in the array
	\param[in] value the initial value of the elements
	*/
	MayaArray(size_type count, const value_type& value=value_type()) : mArray(count, value) {}

	/**
    Creates an array with a copy of the given M***Array instance

	\param[in] maya_array the Maya array to copy
	*/
	MayaArray(const T& maya_array) : mArray(maya_array) {}

	/**
    Creates a copy of another MayaArray instance, copying elements
	from the other array to this array.

	\param[in] other the other MayaArray instance to copy
	*/
	MayaArray(const MayaArray& other) : mArray(other.mArray) {}

	/**
    Assigns all values from a M***Array instance to this array

	\param[in] other the Maya array to assign values from
	*/
	MayaArray& operator=(const T& other) {
		mArray = other;
	}

	/**
    Assigns all values from another MayaArray instance to this array

	\param[in] other the Maya array to assign values from
	*/
	MayaArray& operator=(const MayaArray& other) {
		mArray = other.mArray;
	}

	/**
    Returns an reference to underlying Maya array instance
	*/
	inline T& array() {
		return mArray;
	}

	/**
    Returns an iterator for the first element in the array
	*/
	inline iterator begin() {
		return iterator(mArray);
	}

	/**
    Returns a const iterator for the first element in the array
	*/
	inline const_iterator begin() const {
		return const_iterator(mArray);
	}

	/**
    Returns a const iterator for the first element in the array
	*/
	inline const_iterator cbegin() const {
		return const_iterator(mArray);
	}

	/**
    Returns an iterator for one past the last element in the array
	*/
	inline iterator end() {
		return iterator(mArray, mArray.length());
	}

	/**
    Returns a const iterator for one past the last element in the array
	*/
	const_iterator end() const {
		return const_iterator(mArray, mArray.length());
	}

	/**
    Returns a const iterator for one past the last element in the array
	*/
	inline const_iterator cend() const {
		return const_iterator(mArray, mArray.length());
	}

	/**
    Appends the value to the end of the array

	\param[in] value value to append
	*/
	inline void push_back(const value_type& value) {
		mArray.append(value);
	}

	/**
    Inserts the value in the front of the array

	\param[in] value value to insert
	*/
	inline void push_front(const value_type& value) {
		mArray.insert(value, 0);
	}

	/**
    Clear the array of all elements
	*/
	inline void clear() {
		mArray.clear();
	}

	/**
    Inserts a value at the given iterator position

	\param[in] pos const iterator to insert position
	\param[in] value value to insert at position

	\return
	iterator at the inserted value
	*/
	iterator insert(const_iterator pos, const value_type& value) {
		size_type i = pos - begin();
		mArray.insert(value, i);
		return iterator(mArray, i);
	}

	/**
    Inserts a value before the given iterator position "count" number of times

	\param[in] pos const iterator to insert position
	\param[in] count number of times the value will be inserted
	\param[in] value value to insert at position

	\return
	iterator at the first inserted value
	*/
	iterator insert(const_iterator pos, size_type count, const value_type& value) {
		size_type i = pos - begin();
		while (count--)
			mArray.insert(value, i);
		return iterator(mArray, i);
	}

	/**
    Inserts a range of values before the given iterator position 

	\param[in] pos const interator to the element to insert before
	\param[in] first iterator to first value to insert
	\param[in] last iterator to one past the last value to insert

	\return
	iterator at the first inserted value
	*/
	template<typename InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		size_type i = pos - begin();
		for (size_type p=i; first != last; ++first, ++p)
			mArray.insert(*first, p);
		return iterator(mArray, i);
	}

	/**
    Erases a value from the array at the given iterator position 

	\param[in] pos const iterator to the element to erase

	\return
	iterator at the erase position
	*/
	iterator erase(const_iterator pos) {
		size_type i = pos - begin();
		mArray.remove(i);
		return iterator(mArray, i);
	}

	/**
    Erases a range from the array from the first iterator position up to,
	but not including, the last.

	\param[in] first const iterator to first element to erase
	\param[in] last const iterator to the one past last element to erase

	\return
	iterator at the first erased position
	*/
	iterator erase(const_iterator first, const_iterator last) {
		size_type i = first - begin();
		for (; first != last; ++first)
			mArray.remove(i);
		return iterator(mArray, i);
	}

	/**
    Resizes the array to have the given number of elements. If the array
	size increases, it will be filled with unitialized values. If the
	size decreases, the values at the end will be lost.

	\param[in] count set the size of the array to this number of elements
	*/
	inline void resize(size_type count) {
		mArray.setLength(count);
	}

	/**
    Resizes the array to have the given number of elements. If the array
	size increases, it will be filled with the given value. If the
	size decreases, the values at the end will be lost.

	\param[in] count set the size of the array to this number of elements
	\param[in] value the to set for new elements if the array increases size
	*/
	void resize(size_type count, const value_type& value) {
		size_type oldSize = size();
		if (count < oldSize) {
			mArray.setLength(count);
		}
		else if (oldSize < count) {
			mArray.setLength(count);
			for (size_type i=oldSize; i < count; ++i)
				mArray[i] = value;
		}
	}

	/**
    Returns a reference to the value at the given position. Similar to the
	[] operator, but this will throw a out_of_range exception if the position
	is not within bounds of the array.

	\param[in] pos position in the array

	\return
	reference to element at pos
	*/
	reference at(size_type pos) {
		if (mArray.length() <= pos)
			throw std::out_of_range("MayaArray out of bounds");
		return mArray[pos];
	}

	/**
    Returns a const reference to the value at the given position. Similar to the
	[] operator, but this will throw a out_of_range exception if the position
	is not within bounds of the array.

	\param[in] pos position in the array

	\return
	const_reference to element at pos
	*/
	const_reference at(size_type pos) const {
		if (mArray.length() <= pos)
			throw std::out_of_range("MayaArray out of bounds");
		return mArray[pos];
	}

	/**
    Returns a reference to the value at the given position. Similar to the
	"at" function, but there is no bounds checking before getting the value.

	\param[in] pos position in the array

	\return
	reference to value at pos
	*/
	inline reference operator[](size_type pos) {
		return mArray[pos];
	}

	/**
    Returns a const reference to the value at the given position. Similar to the
	"at" function, but there is no bounds checking before getting the value.

	\param[in] pos position in the array

	\return
	const_reference to value at pos
	*/
	inline const_reference operator[](size_type pos) const {
		return mArray[pos];
	}

	/**
    Returns a reference to the first value in the array

	\return
	reference to value
	*/
	inline reference front() {
		return (*begin());
	}

	/**
    Returns a const reference to the first value in the array

	\return
	const_reference to value
	*/
	inline const_reference front() const {
		return (*begin());
	}

	/**
    Returns a reference to the last value in the array

	\return
	reference to value
	*/
	inline reference back() {
		return (*(end() - 1));
	}

	/**
    Returns a const reference to the last value in the array

	\return
	const_reference to value
	*/
	inline const_reference back() const {
		return (*(end() - 1));
	}

	/**
    Returns a size of the array

	\return
	number of elements in array
	*/
	inline size_type size() const {
		return mArray.length();
	}
};

} // namespace mayaarray

#endif // MAYAARRAY_MAYA_ARRAY_H_