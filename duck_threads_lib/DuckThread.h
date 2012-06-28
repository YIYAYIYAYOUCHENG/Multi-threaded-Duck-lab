#ifndef _DuckThread_h__
#define _DuckThread_h__

#include <boost/thread.hpp>
#include "DuckBuffer.h"
#include "DuckException.h"
#include <iostream>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/**
* @brief the family of variant duck threads are derived from DuckThread
*/
class DuckThread {
public:

	/**
	* @brief to start the thread of operation
	*/
	virtual boost::thread* launch(void) = 0;
};

/**
* @brief DuckLoopThread provides with the typical operations in duck-lab
*		- a job is repeated for a number of loops or endlessly.
*/
class DuckLoopThread : public DuckThread {
public:
	/**
	* @param nums the repeating times of job operation; 
	* if it is 0, the operation will be repeated forever.
	*/
	DuckLoopThread(unsigned long long nums) : max_loop_counts(nums) {}
	/**
	* by default, job operation will be repeated infinite times
	*/
	DuckLoopThread(void) : max_loop_counts(0) {}
	/**
	* @brief job operation is where the real work is performed
	*/
	virtual void job(void) = 0;
	/**
	* @brief task calls the job method and is the start point when
	* launching a thread of execution.
	*/
	void task(void) {
		int duck_running_loop = 1;
		unsigned long long loop_count = 0;
		while( duck_running_loop) {

			try {
				job();
			} catch (DuckException &e) {
				std::cout<< e.what() <<std::endl;
				//std::cout<<e<<std::endl;
				break;
			}

			if( max_loop_counts) {
				loop_count ++;
				if( loop_count >= max_loop_counts)
					duck_running_loop = 0;
			}
		}
	}
	/**
	* @brief the starting point of the thread
 	*/	
	boost::thread* launch(void) {
		return new boost::thread(&DuckLoopThread::task, this);
	}
protected:
	/**
	* @brief max_loop_counts defines how many times job() method will
	* be called inside task(); if it is 0, job() will be called infinite
	* times.
	*/
	unsigned long long max_loop_counts;	
				
};
	
#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
	
