#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <assert.h>

class ThreadPool
{
public:
				ThreadPool(size_t threadCount);
				~ThreadPool();
				template <class F, class... Args>
	void		enqueueJob(F&& f, Args&&... args);
	bool		jobDone();
private:
	void		workerThread();
private:
	std::vector<std::thread>			_workerThread;
	std::queue<std::function<void()>>	_jobs;
	std::condition_variable				_conditionVar;
	std::mutex							_mutex;

	size_t		_threadCount;
	int			_workingCount;
	bool		_stopAll;
};

template<class F, class ...Args>
inline void ThreadPool::enqueueJob(F&& f, Args && ...args)
{
	if (_stopAll)
		assert(false);

	//using returnType = typename std::invoke_result<F(Args...)>::type;

	auto job = (std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	{
		std::lock_guard<std::mutex> lock(_mutex);
		_jobs.push([job]() {job(); });
		//_jobs.push([job]() { });
	}

	_conditionVar.notify_one();
}