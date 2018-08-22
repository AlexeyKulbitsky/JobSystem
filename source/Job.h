#ifndef SHADOW_JOB_INCLUDE
#define SHADOW_JOB_INCLUDE

#include <atomic>

namespace sh
{

namespace jobs
{
    struct Job;
	typedef void(*JobFunction)(Job* job, void *args);
    typedef void(*ParallelFunction)(int* array, int count);

	struct Job
	{
        Job* parent = nullptr;
		JobFunction function = nullptr;
		void* args = nullptr;
        std::atomic<int> unfinishedJobs = {0};
	};
    
    
    Job* CreateJob(JobFunction function);
    Job* CreateChildJob(Job* parent, JobFunction function);
    Job* ParallelFor(ParallelFunction, int* array, int count);
    void Finish(Job* job);
    
} // jobs

} // sh

#endif
