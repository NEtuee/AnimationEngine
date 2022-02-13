#include "ThreadPool.h"
#include <assert.h>

ThreadPool::ThreadPool(size_t threadCount)
	:_threadCount(threadCount), _workingCount(0), _stopAll(false)
{
	_workerThread.reserve(_threadCount);
	for (size_t i = 0; i < threadCount; ++i)
	{
		_workerThread.emplace_back([this]() {this->workerThread(); });
	}
}

ThreadPool::~ThreadPool()
{
	_stopAll = true;
	_conditionVar.notify_all();

	for (auto& t : _workerThread)
	{
		t.join();
	}
}

bool ThreadPool::jobDone()
{
	return _jobs.size() == 0 && _workingCount == 0;
}

void ThreadPool::workerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_conditionVar.wait(lock, [this]() {return !this->_jobs.empty() || _stopAll; });
		if (_stopAll && this->_jobs.empty())
			return;

		std::function<void()> job = std::move(_jobs.front());
		_jobs.pop();
		_workingCount++;
		lock.unlock();

		job();

		lock.lock();
		--_workingCount;
		lock.unlock();
	}

}