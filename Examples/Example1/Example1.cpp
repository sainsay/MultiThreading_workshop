#include <thread>
#include <iostream>

//thread creation

void Task1(){
	std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
}

int main( int argn, char** args ){

	auto start_time = std::chrono::high_resolution_clock::now();

	std::thread my_thread = std::thread( Task1 );
	my_thread.join();

	auto stop_time = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count() << " milliseconds" << std::endl;

	std::getchar();
	return 0;
}