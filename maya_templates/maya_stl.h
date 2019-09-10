/*
The MIT License (MIT)

Copyright (c) 2019 Scott Englert

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
#ifndef MAYATEMPLATES_MAYA_STL_H_
#define MAYATEMPLATES_MAYA_STL_H_

#include <functional>
#include <maya/MString.h>
#include <maya/MUniqueString.h>

namespace std {
	/*
	Standard Template Library hash specialization for MStrings
	*/
	template<>
	class hash<MString>
	{
	public:
		size_t operator() (const MString& value) const
		{
			MUniqueString uniqueString = MUniqueString::intern(value);
			return uniqueString.hash();
		}
	};

	/*
	Standard Template Library equal_to specialization for MStrings
	*/
	template <>
	class equal_to<MString>
	{
	public:
		bool operator() (const MString& valueA, const MString& valueB) const
		{
			return valueA == valueB;
		}
	};
} // std namespace

#endif // MAYATEMPLATES_MAYA_STL_H_