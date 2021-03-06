#include <thread>
#include <iostream>
#include <string_view>
#include <array>
#include <mutex>

// thread data transfer. receiver waits

class Resource
{
public:
	Resource() : my_int( -1 ){}

	void give( int i ){
		resource_mutex.lock();
		my_int = i;
		resource_mutex.unlock();
		cv.notify_all();
	}
	int take(){
		std::cout << "I wait until the resource has a value" << std::endl;
		std::unique_lock lk( resource_mutex );
		cv.wait( lk );
		int temp = my_int;
		my_int = -1;
		lk.unlock();
		return temp;
	}
private:
	std::condition_variable cv;
	int my_int;
	std::mutex resource_mutex;
};

void Task1( Resource* res ){
	while( true )
	{
		res->give( 5 );
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
	}
}

void Task2( Resource* res ){
	while( true )
	{
		int i = res->take();

		if( i == -1 )
		{
			std::cout << "resource empty. that is not good!" << std::endl;
		}
		else
		{
			std::cout << "value is: " << i << std::endl;
		}
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}
}

int main( int argn, char** args ){

	auto start_time = std::chrono::high_resolution_clock::now();

	Resource res;

	std::array<std::thread, 2> my_threads
	{
		std::thread( Task1, &res ),
		std::thread( Task2, &res ),
	};

	for( auto& thread : my_threads )
	{
		thread.join();
	}

	auto stop_time = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast< std::chrono::milliseconds >( stop_time - start_time ).count() << " milliseconds" << std::endl;

	std::getchar();
	return 0;
}