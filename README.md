# Maya API Utils
This repo contains useful utilties for Autodesk's Maya API in C++. More common tools will be added over time.

## Maya Iteration
Contains tools for iteration. Currently a template class to make working with Maya's arrays more like standard library iterators.

### Usage Examples
`MPointArray myPointArray(5); // array of 5 points
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
`
