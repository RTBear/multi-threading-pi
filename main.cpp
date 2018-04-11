#include <iostream>
#include <thread>//std::thread std::lock_guard std::thread::hardware_concurrency();
#include <mutex>
#include <vector>
#include <memory>
#include <queue> //std::queue
#include <unordered_map> //std::unordered_map

#include "computePi.hpp"

class TaskList{
	public:
		TaskList(int length){
			for(int i = 1; i <= length; i++){m_q.push(i);}//fill m_q
		}
		int getDigitToCompute(){
			try{
				std::lock_guard<std::mutex> lck (m_mtx); 
				m_digit = m_q.front();
				m_q.pop();
				return m_digit;
			}catch(std::exception e){return -1;}
		}
	private:
		int m_digit;
		std::queue<int> m_q;
		std::mutex m_mtx;
};

class ResultTable{
	public:
		void addToTable(int key, int value){
			try{
				std::lock_guard<std::mutex> lck (m_mtx);
				m_table.insert({key,value});
			}catch(std::exception e){}
		}
		int getValueAt(int key){
			try{
				std::lock_guard<std::mutex> lck (m_mtx);
				return m_table.at(key);
			}catch(std::exception e){return -1;}
		}
	private:
		std::unordered_map<int,int> m_table;
		std::mutex m_mtx;
};

void threadWorker(std::uint16_t threadNum, TaskList& tasks, ResultTable& results) {
	//#warning TODO: This function will take a reference to the FIFO task queue as an argument
	//#warning TODO: This function will take a reference to your unordered_map as an argument

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
	//#warning TODO: Initialize your thread-safe data structures here

	TaskList lst(1000);//create a task queue of length 1000
	ResultTable results;

	//
	// Make as many threads as there are CPU cores
	// Assign them to run our threadWorker function, and supply arguments as necessary for that function
	std::vector<std::shared_ptr<std::thread>> threads;
	for (std::uint16_t core = 0; core < std::thread::hardware_concurrency(); core++)
		// The arguments you wish to pass to threadWorker are passed as
		// arguments to the constructor of std::thread
		threads.push_back(std::make_shared<std::thread>(threadWorker, core, lst, results));

	//
	// Wait for all of these threads to complete
	for (auto&& thread : threads)
		thread->join();

	std::cout << std::endl << std::endl;

#warning TODO: Print the digits of PI from our unordered_map, in order

	return 0;
}
