#include <iostream>
#include <thread>//std::thread std::lock_guard std::thread::hardware_concurrency();
#include <mutex>
#include <vector>
#include <memory>
#include <queue> //std::queue
#include <unordered_map> //std::unordered_map

#include "computePi.hpp"

class ResultTable{
	public:
		ResultTable(){}
		ResultTable(const ResultTable& other) {
			std::lock_guard<std::mutex> lock(other.m_mtx);
			m_table = other.m_table;
		}

		void addToTable(int key, unsigned int value){
			std::lock_guard<std::mutex> lck (m_mtx);
			m_table.insert({key,value});
		}
		int getValueAt(int key){
			std::lock_guard<std::mutex> lck (m_mtx);
			return m_table.at(key);
		}
		std::unordered_map<int,int> m_table;
		mutable std::mutex m_mtx;
};

class TaskList{
	public:
		TaskList(int length){
			for(int i = 1; i <= length; i++){m_q.push(i);}//fill m_q
		}
		TaskList(const TaskList& other) {
			std::lock_guard<std::mutex> lock(other.m_mtx);
			m_q = other.m_q;
			m_digit = other.m_digit;
		}
		int getDigitToCompute(){
			std::lock_guard<std::mutex> lck (m_mtx); 
			m_digit = m_q.front();
			m_q.pop();
			return m_digit;
		}
		bool tasksLeft(){
			std::lock_guard<std::mutex> lck (m_mtx); 
			if(m_q.size() > 0){
				return true;
			}else{
				return false;
			}
		}
		bool consume(){
			while(tasksLeft()){
				int digit = getDigitToCompute(); 
				results.addToTable(digit, computePiDigit(digit));
				std::cout << ".";
				std::cout.flush();
			}
		}
		ResultTable results;
		int m_digit;
		std::queue<int> m_q;
		mutable std::mutex m_mtx;
};

void threadWorker(std::uint16_t threadNum, TaskList &tasks) {
	tasks.consume();
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
		threads.push_back(std::make_shared<std::thread>(threadWorker, core, std::ref(lst)));

	//
	// Wait for all of these threads to complete
	for (auto&& thread : threads)
		thread->join();

	std::cout << std::endl << std::endl;
	std::cout << "3.";
	for(int i = 1; i <= lst.results.m_table.size(); i++){
		std::cout << lst.results.getValueAt(i);
	}
	std::cout << std::endl;

	return 0;
}
