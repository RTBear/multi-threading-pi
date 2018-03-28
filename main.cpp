#include <iostream>
#include <thread>
#include <vector>
#include <memory>

#include "computePi.hpp"


void threadWorker(std::uint16_t threadNum) {
#warning TODO: This function will take a reference to the FIFO task queue as an argument
#warning TODO: This function will take a reference to your unordered_map as an argument

    //
    // This code exists simply to illustrate a worker thread.
    // I had better not see this in your final submission.
    std::cout << "Hi! I'm thread number " << threadNum << ", and I can count to 10!\n";
    for (int i = 1; i <= 10; ++i)
		std::cout << "[" <<  threadNum << "] " << i << std::endl;

    std::cout << "[" <<  threadNum << "] watch me compute digit #"
        << threadNum+1 << " of pi: "
        << computePiDigit(threadNum+1) << std::endl;
}



int main() {
#warning TODO: Initialize your thread-safe data structures here

	//
	// Make as many threads as there are CPU cores
    // Assign them to run our threadWorker function, and supply arguments as necessary for that function
	std::vector<std::shared_ptr<std::thread>> threads;
	for (std::uint16_t core = 0; core < std::thread::hardware_concurrency(); core++)
        // The arguments you wish to pass to threadWorker are passed as
        // arguments to the constructor of std::thread
		threads.push_back(std::make_shared<std::thread>(threadWorker, core));

	//
	// Wait for all of these threads to complete
	for (auto&& thread : threads)
		thread->join();

	std::cout << std::endl << std::endl;

#warning TODO: Print the digits of PI from our unordered_map, in order

	return 0;
}
