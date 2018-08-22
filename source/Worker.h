#ifndef SHADOW_WORKER_INCLUDE
#define SHADOW_WORKER_INCLUDE

#include "JobQueue.h"

#include <thread>
#include <atomic>

namespace sh
{

namespace jobs
{
	class JobSheduler;

	class Worker
	{
	public:
		enum class Type
		{
			Background,
			Foreground
		};

		Worker() = default;
		Worker(const Worker& deleted) = delete;
		Worker(Worker&& deleted) = delete;
		Worker(Type type, JobSheduler* scheduler);

		void Start();
		void Stop();
		bool IsRunning() const;
		
		void Submit(Job* job);
		void Wait(Job* job);

		std::thread::id GetThreadId() const;
		Type GetType() const { return m_type; }
        Job* AlocateJob();

	private:
		void ThreadFuction();
		Job* GetJob();

	private:
		JobSheduler* m_scheduler = nullptr;
		bool m_isRunning = false;
		Type m_type;

		JobQueue m_jobQueue;
		std::thread m_thread;
		std::thread::id m_threadId;
        
        const size_t jobsCount = 50000U;
        Job* jobs = nullptr;
        size_t jobIndex = 0U;
        friend class JobSheduler;
	};

} // jobs

} // sh

#endif
