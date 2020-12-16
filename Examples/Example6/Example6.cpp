#include <thread>
#include <iostream>
#include <string_view>
#include <array>
#include <mutex>

// mutex. solve race conditions


//#define ALTERNATIVE

class Counter
{
public:
	Counter() : count( 0 ), count_mutex(){}
#ifndef ALTERNATIVE
	void Add( int value ){
		count_mutex.lock();

		for( size_t i = 0; i < value; i++ )
		{
			count++;
		}

		count_mutex.unlock();
	}
#endif
#ifdef ALTERNATIVE
	void Add( int value ){
		std::lock_guard guard( count_mutex );

		for( size_t i = 0; i < value; i++ )
		{
			count++;
		}
}
#endif

	int get_count(){
		return count;
	}

private:
	int count;
	std::mutex count_mutex;
};

void Task1( Counter* counter ){
	counter->Add( 5000 );
}

int main( int argn, char** args ){

	auto start_time = std::chrono::high_resolution_clock::now();

	Counter my_counter;

	std::array<std::thread, 5> my_threads
	{
		std::thread( Task1, &my_counter ),
		std::thread( Task1, &my_counter ),
		std::thread( Task1, &my_counter ),
		std::thread( Task1, &my_counter ),
		std::thread( Task1, &my_counter )
	};

	for( auto& thread : my_threads )
	{
		thread.join();
	}

	std::cout << "The counter counted to: " << my_counter.get_count() << std::endl;

	auto stop_time = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast< std::chrono::milliseconds >( stop_time - start_time ).count() << " milliseconds" << std::endl;

	std::getchar();
	return 0;
}