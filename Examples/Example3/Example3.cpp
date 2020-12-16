#include <thread>
#include <iostream>


void Task1(){
	std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
	std::cout << "done with Task 1!" << std::endl;
}

void Task2(){
	std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
	std::cout << "done with Task 2!" << std::endl;
}

int main( int argn, char** args ){

	auto start_time = std::chrono::high_resolution_clock::now();

	std::thread my_thread_1 = std::thread( Task1 );
	std::thread my_thread_2 = std::thread( Task2 );

	my_thread_1.join();
	my_thread_2.join();

	auto stop_time = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast< std::chrono::milliseconds >( stop_time - start_time ).count() << " milliseconds" << std::endl;

	std::getchar();
	return 0;
}