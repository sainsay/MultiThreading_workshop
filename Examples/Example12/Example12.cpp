#include <thread>
#include <array>
class Scene;
#define IMAGE_SIZE 512

// pseudo code ray tracing multithreading example

struct pixel{
	__int8 r;
	__int8 g;
	__int8 b;
};

struct Job{
	std::array<pixel, IMAGE_SIZE * IMAGE_SIZE>* const image_out;
	Scene* const scene;
	const int starting_row;
	const int row_count;
	const int image_width;
};

void lets_render_it(Job my_job) noexcept {
	for( size_t i = my_job.starting_row; i < my_job.starting_row + my_job.row_count; i++ )
	{
		for( size_t j = 0; j < my_job.image_width; j++ )
		{
			// check intersection here with scene provided by job
			my_job.image_out->at(i* IMAGE_SIZE + j) = pixel{ 40,30,12 };
		}
	}
}

int main(int, char**){

	std::array<pixel, IMAGE_SIZE * IMAGE_SIZE> image;
	Scene* my_scene;
	//going to have 4 threads
	const int row_per_thread = IMAGE_SIZE / 4;

	std::thread thread1 = std::thread( lets_render_it, Job{ &image, my_scene, 0, row_per_thread, IMAGE_SIZE } );
	std::thread thread2 = std::thread( lets_render_it, Job{ &image, my_scene, 0 + row_per_thread, row_per_thread, IMAGE_SIZE } );
	std::thread thread3 = std::thread( lets_render_it, Job{ &image, my_scene, 0 + row_per_thread * 2, row_per_thread, IMAGE_SIZE } );
	std::thread thread4 = std::thread( lets_render_it, Job{ &image, my_scene, 0 + row_per_thread * 3, row_per_thread, IMAGE_SIZE } );

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();

	return 0;
}



