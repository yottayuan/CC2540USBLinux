/*
 * Singleton.h
 *
 *  Created on: Apr 2, 2014
 *      Author:
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_
#include <cstddef>

namespace framework {
template<class T>
class Singleton {
private:
	static T* mInstance;
public:
	static T* Instance() {
		if (!mInstance) {
			mInstance = new T;
		}
		return mInstance;
	}
};

template<class T> T* Singleton<T>::mInstance = NULL;

}

#endif /* SINGLETON_H_ */
