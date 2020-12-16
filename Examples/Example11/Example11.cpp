#include <thread>
#include <iostream>
#include <string_view>
#include <array>
#include <mutex>
#include <vector>

// thread data transfer. buffers

class Resource
{
public:

	void give( int i ){
		std::lock_guard guard( resource_mutex );
		my_ints.push_back(i);
	}

	std::vector<int> take(){
		std::lock_guard guard( resource_mutex );

		std::vector<int> temp = std::vector<int>(  my_ints ); // does a deep copy (but always check with your data!!)
		my_ints.clear();

		return std::move(temp); // suggest a move over a copy

		// NOTE: copy and clear of vector might not be needed but that is outside of the workshops scope. for now
	}
private:
	std::condition_variable cv;
	std::vector<int> my_ints;
	std::mutex resource_mutex;
};

void Task1( Resource* res ){
	while( true )
	{
		res->give( std::rand());
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}
}

void Task2( Resource* res ){
	while( true )
	{
		auto i = res->take();

		if( i.empty() )
		{
			std::cout << "resource empty. that is not good!" << std::endl;
		}
		else
		{
			std::cout << "value is: ";
			for( std::vector<int>::const_iterator it = i.begin(); it != i.end(); ++it )
				std::cout << *it << ' ';
			std::cout << std::endl;
		}
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
	}
}

int main( int argn, char** args ){

	auto start_time = std::chrono::high_resolution_clock::now();

	Resource res;

	std::array<std::thread, 5> my_threads
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