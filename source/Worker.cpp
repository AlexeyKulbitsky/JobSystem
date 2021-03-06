#include "Worker.h"
#include "Job.h"
#include "JobScheduler.h"

#include <iostream>

namespace sh
{

namespace jobs
{

	Worker::Worker(Worker::Type type, JobSheduler* scheduler)
		: m_scheduler(scheduler)
		, m_type(type)
        , m_jobQueue(150000U)
	{
		if (m_type == Type::Foreground)
		{
			m_threadId = std::this_thread::get_id();
		}
        //jobs.resize(jobsCount);
        jobs = new Job[jobsCount];
	}

	void Worker::Start()
	{
		if (m_isRunning)
			return;

		m_isRunning = true;
		m_thread = std::thread(&Worker::ThreadFuction, this);
		m_threadId = m_thread.get_id();
	}

	void Worker::Stop()
	{
		if (!m_isRunning)
			return;

		m_isRunning = false;
		m_thread.join();
	}

	bool Worker::IsRunning() const
	{
		return m_isRunning;
	}

	void Worker::Submit(Job* job)
	{
		m_jobQueue.Push(job);
	}

	void Worker::Wait(Job* job)
	{

	}

	std::thread::id Worker::GetThreadId() const
	{
		return m_threadId;
	}

    Job* Worker::AlocateJob()
    {
        Job* result = &jobs[jobIndex++];
        if (jobIndex == jobsCount)
            jobIndex = 0U;
        return result;
    }
    
	void Worker::ThreadFuction()
	{
		while (m_isRunning)
		{
			Job* job = GetJob();
			if (job)
			{
				job->function(job, job->args);
                Finish(job);
			}
		}
	}

	Job* Worker::GetJob()
	{
		Job* job = m_jobQueue.Pop();

		if (job != nullptr)
		{
			return job;
		}
		else
		{
			Worker* worker = m_scheduler->GetRandomWorker();
			
			if (worker != this)
			{
				Job* job = worker->m_jobQueue.Steal();

				if (job != nullptr)
				{
					//std::cout << "Job stolen from another worker\n";
					return job;
				}
				else
				{
					std::this_thread::yield();
					return nullptr;
				}
			}
			else
			{
				std::this_thread::yield();
				return nullptr;
			}
			
		}
	}

} // jobs

} // sh
