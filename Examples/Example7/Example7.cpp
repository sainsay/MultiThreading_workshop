#include <thread>
#include <iostream>
#include <string_view>
#include <array>
#include <mutex>

// deadlocking

class Deadlock
{
public:
	void do_stuff_with(Deadlock* deadlock){
		std::lock_guard guard( mutex );
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
		deadlock->do_stuff();
	}
	void do_stuff(){
		std::lock_guard guard( mutex );
		std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
	}

private:

	std::mutex mutex;
};

void Task1( Deadlock* deadlock_a, Deadlock* deadlock_b ){
	deadlock_a->do_stuff_with(deadlock_b);
}

void Task2( Deadlock* deadlock_a, Deadlock* deadlock_b ){
	deadlock_b->do_stuff_with( deadlock_a );
}

int main( int argn, char** args ){

	auto start_time = std::chrono::high_resolution_clock::now();

	Deadlock deadlock_a;
	Deadlock deadlock_b;

	std::array<std::thread, 2> my_threads
	{
		std::thread( Task1, &deadlock_a, &deadlock_b ),
		std::thread( Task2, &deadlock_a, &deadlock_b ),
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