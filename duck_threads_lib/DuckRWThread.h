#ifndef _DuckRWThread_h__
#define _DuckRWThread_h__

#include "DuckThread.h"
#include <boost/thread.hpp>
#include <vector>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/**
* @brief this defines such an intermediate unit:
*    it reads from a duck buffer; then, after some operations, it writes
*    results to a set of other duck buffers
* @tparam T1 the type of elements in input duck buffer
* @tparam Mid the type of objects who bridge the input and output buffer 
* @tparam T2 the type of elements in output duck buffers
*/
template< class T1, class Mid, class T2>
class DuckRWThread : public DuckLoopThread {
public:
	/**
	* a constructor
	*/
	DuckRWThread(DuckBuffer* src_duck_buffer_, Mid *mid_, 
		std::vector<DuckBuffer*> *dest_duck_buffers, int loops)
		: DuckLoopThread(loops), 
			src_duck_buffer(src_duck_buffer_), mid(mid_)
	{
		std::vector<DuckBuffer*>::iterator i = 
						dest_duck_buffers->begin();
                for( ; i != dest_duck_buffers->end(); i++)
                        duck_buffers.push_back(*i);

	} 
	/**
	* a constructor
	*/
	DuckRWThread(DuckBuffer* src_duck_buffer_, Mid *mid_, 
			std::vector<DuckBuffer*> *dest_duck_buffers)
		: src_duck_buffer(src_duck_buffer_), mid(mid_)
	{
		std::vector<DuckBuffer*>::iterator i = 
						dest_duck_buffers->begin();
                for( ; i != dest_duck_buffers->end(); i++)
                        duck_buffers.push_back(*i);

	} 
	/**
	* @brief to read an element from the input buffer, operate on it,
	* then write it into the output
	*/
	void job(void) {
		std::vector<DuckBuffer *>::iterator it;
		T1 elem1;
		T2 elem2;

		/**
		* to read an element from the input buffer
		*/
		src_duck_buffer->read(&elem1);
		/**
		* The intermediate unit will deal with the input and
		* generate a result, which will be distributed into each 
		* output buffer registered, i.e. each buffer in duck_buffers
		* will get a copy of the result.
		*/
		elem2 = mid->put_get(elem1);
		for( it = duck_buffers.begin();
			it != duck_buffers.end(); ++it)
			(*it)->write(&elem2);
	}

protected:
	/**
	* @brief the source buffer 
	*/
	DuckBuffer *src_duck_buffer;
	/**
	* @brief the intermediate object which bridges input and output buffers
	*/
	Mid *mid;
	/**
	* @brief a vector of output buffers
	*/
	std::vector<DuckBuffer*> duck_buffers;
};
#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
