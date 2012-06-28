#ifndef _DuckBuffer_h__
#define _DuckBuffer_h__

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <deque>
#include <queue>
#include <iostream>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/**
* @brief abstract buffer between producer and consumer threads
*/
class DuckBuffer {
public:
	/**
	* @brief to write an element in the buffer
	*/
	virtual void write(void* elem) = 0;
	/**
	* @brief to read an element in the buffer
	*/
	virtual void read(void* res) = 0;
};


/**
* @brief DuckBuffer can be used whenever a buffer is needed.
*	 
*	 DuckBuffer is a one producer one consumer buffer.
*	 Inside buffer, necessary synchronization logic is implemented.
*	 Specific or more complex buffer can inherit DuckBuffer.
*/	 
template<class T>
class DuckBufferTemplate : public DuckBuffer {
public:
	DuckBufferTemplate(void)
		: available(0) {}
	/**
	* @brief a copy of *(T*)elem will be put into the buffer
	*/
	void write(void* elem);
	/**
	* @brief remove the first element, whose copy will be
	*	put in *(T*)res, in the buffer.
	*/
	void read(void* res);	
protected:
	/**
	* @brief the real buffer where elements are stored
	*/
	std::queue<T> buffer;
	/**
	* @brief a mutex used for basic synchronization
	*/
	boost::mutex mutex;
	boost::condition_variable cond;
	/**
	* @brief the number of elements in the buffer
	*/
	int available;
};

template<class T>
void DuckBufferTemplate<T>::write(void* element) 
{
	T elem = *(T*)element;
	boost::unique_lock<boost::mutex> lock(mutex);
	available ++;
	if( available > 2)
		lock.unlock();
	buffer.push(elem);
	cond.notify_one();
}	

template<class T>
void DuckBufferTemplate<T>::read(void* res)
{
	T elem;
	boost::unique_lock<boost::mutex> lock(mutex);
	while( available == 0) {
		cond.wait(lock);
	}
	available --;
	if( available != 0)
		lock.unlock();
	elem = buffer.front();
	buffer.pop();
	*(T*)res = elem;
}
#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
