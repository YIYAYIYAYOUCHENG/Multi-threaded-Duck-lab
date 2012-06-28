#ifndef _DuckReadingThread_h__
#define _DuckReadingThread_h__

#include "DuckThread.h"
#include <boost/thread.hpp>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/**
* @brief this is the default reading thread in duck-lab
* @tparam  T the type of element in the duck buffer
* @tparam Dest is the type of destenation where the element read 
*  from buffer is put
*/
template<class T, class Dest>
class DuckReadingThread : public DuckLoopThread {
public:
	/**
	* @param duck_buffer_ the duck buffer this thread read from
	* @param dest_ the read element is put in dest_
	* @param loops the repeating numbers for job()
	*/ 
	DuckReadingThread(DuckBuffer *duck_buffer_, Dest *dest_, 
						unsigned long long loops)
		: DuckLoopThread(loops), duck_buffer(duck_buffer_), dest(dest_)
	{}
	/**
	* by default the job is executed forever
	*/
	DuckReadingThread(DuckBuffer *duck_buffer_, Dest *dest_)
		: duck_buffer(duck_buffer_), dest(dest_) {}
	/**
	* @brief the reading operation is defined in job()
	*/
	void job(void) {
		T elem;

		duck_buffer->read(&elem);
		dest->put(elem);
	}

protected:
	/**
	* @brief input buffer
	*/
	DuckBuffer *duck_buffer;
	/**
	* @brief the destination where the element is put
	*/
	Dest *dest;

};

#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
