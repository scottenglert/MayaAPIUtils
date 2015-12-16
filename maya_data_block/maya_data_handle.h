#include <maya/MDataHandle.h>

template<typename T>
struct MayaDataHandleIO {
	static inline void get(const MDataHandle& dataHandle) { }
	static inline void set(MDatahandle& dataHandle, T& value) { }
};

template<>
struct MayaDataHandleIO<double> {
	static inline double get(const MDataHandle& dataHandle) {
		return dataHandle.asDouble();
	}

	static inline void set(MDataHandle& dataHandle, const double& value) {
		dataHandle.setDouble(value);
	}
		

};