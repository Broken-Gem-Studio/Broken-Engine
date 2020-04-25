#include "ModuleThreading.h"
#include "Optick/include/optick.h"
#include "mmgr/nommgr.h"


using namespace Broken;
ModuleThreading::ModuleThreading(bool start_enabled) : Module(start_enabled) {
	name = "Threading";
}

ModuleThreading::~ModuleThreading() {
}

bool ModuleThreading::Init(json& file) {
	concurrentThreads = std::thread::hardware_concurrency();
	return true;
}

bool ModuleThreading::Start() {
	for (int i = 0; i < concurrentThreads - 1; i++) {
		threadStatus.push_back(false);
		threadVector.push_back(std::thread(&ModuleThreading::ProcessTasks, this, i, std::ref(stopPool)));
		threadNames.push_back("Worker thread " + std::to_string(i + 1));
	}

	ENGINE_AND_SYSTEM_CONSOLE_LOG("Created %d threads.", concurrentThreads - 1);

	poolTerminated = false;
	stopPool = false;

	return true;
}

update_status ModuleThreading::Update(float dt) {
	FinishProcessing();
	return UPDATE_CONTINUE;
}

update_status ModuleThreading::PostUpdate(float dt) {
	FinishProcessing();
	return UPDATE_CONTINUE;
}

bool ModuleThreading::CleanUp() {
	if (!poolTerminated)
		ShutdownPool();

	threadNames.clear();
	threadStatus.clear();
	threadVector.clear();
	return true;
}

void ModuleThreading::AddTask(std::function<void()> newTask) {
	{
		std::lock_guard<std::mutex> lk(tQueueMutex);
		tasksQueue.push(newTask);
	}
	condition.notify_one(); //Waking up a thread to process the new task
}

void ModuleThreading::ShutdownPool() {
	{
		std::lock_guard<std::mutex> lk(threadPoolMutex);
		stopPool = true;
	}

	condition.notify_all(); //Waking up all threads

	//Joining all threads
	for (std::thread& currentThread : threadVector)
		currentThread.join();

	threadVector.empty();
	threadStatus.empty();
	poolTerminated = true;
	ENGINE_AND_SYSTEM_CONSOLE_LOG("Thread pool has been shutdown, all %d threads are joined.", concurrentThreads);
}

void ModuleThreading::ProcessTasks(int threadID, std::atomic<bool>& stop) {
	OPTICK_THREAD(threadNames[threadID].c_str());
	while (true) {
		{
			std::lock_guard<std::mutex> lk(threadPoolMutex);
			threadStatus[threadID] = false;
		}

		std::function<void()> Task;

		{
			std::unique_lock<std::mutex> lock(tQueueMutex);
			condition.wait(lock, [&] { return !tasksQueue.empty() || stop; });

			if (stop) //If we need to stop the thread, we break the infinite loop
				break;

			{
				std::lock_guard<std::mutex> lk(threadPoolMutex);
				threadStatus[threadID] = true;
				//ENGINE_CONSOLE_LOG("Processing task in thread %d", threadID + 1);
			}

			Task = tasksQueue.front();
			tasksQueue.pop();
		}
		OPTICK_EVENT("Processing a task");
		Task();
	}
}

void ModuleThreading::FinishProcessing() {
	bool processing = true;
	while (processing) {
		tQueueMutex.lock();
		//If our task queue is empty we check that our thread pool has finished processing
		if (tasksQueue.empty()) {
			OPTICK_CATEGORY("Waiting for threads", Optick::Category::Wait);
			tQueueMutex.unlock();
			std::unique_lock<std::mutex> threadPoolLock(threadPoolMutex);

			bool threadsProcessing = false;
			for (int i = 0; i < threadVector.size() && !threadsProcessing; ++i)
				threadsProcessing = threadsProcessing || threadStatus[i];

			processing = threadsProcessing;
		}
		//Otherwise we process a task ourselves
		else {
			OPTICK_EVENT("Processing a task");

			//ENGINE_CONSOLE_LOG("Processing a task on main thread");
			std::function<void()> Task;
			Task = tasksQueue.front();
			tasksQueue.pop();
			tQueueMutex.unlock();
			Task();
		}
	}
}
