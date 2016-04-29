# Maya API Utils
This repo contains useful utilties for Autodesk's Maya API in C++. More common tools will be added over time.

## Maya Array
A class template that uses a Maya M***Array as the underlying container but has a more standard library compatible interface. It provides the best of both worlds by still being able to use it in Maya's API but also with other libraries and algorithms without copying the data from one container to the other. More improvements and support will be when possible.

### Usage Examples

```
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
for(auto& pnt : meshPoints)
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
```

## Maya Iteration
Contains tools for iteration. Currently available is class template that wraps an existing Maya M***Array object and provides a standard library iterator. This makes it easy to pass our Maya array objects to other libraries and algorithms that work with iterators without having to copy your data to another compatible container.

### Usage Examples
```
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
	std::cout << *it << std::endl;
	
// using new range based loop in C++11
for(auto pnt : myPointArrayRange)
	 std::cout << pnt << std::endl;
	 
// sort the array using a comparsion function
std::sort(beginIt, endIt, myPointCompareFunc);

// getting the number of points between two iterators
mayaiteration::MayaArrayRange<MPointArray>::iterator::difference_type dist = std::distance(beginIt, endIt);
std::cout << dist << std::endl;
```
