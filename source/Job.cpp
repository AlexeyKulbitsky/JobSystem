#include "Job.h"
#include "JobScheduler.h"

namespace sh
{
    
namespace jobs
{
    Job* CreateJob(JobFunction function)
    {
        Job* job = JobSheduler::getInstance().GetCurrentThreadWorker()->AlocateJob();
        //Job* job = new Job();
        job->parent = nullptr;
        job->function = function;
        job->args = nullptr;
        job->unfinishedJobs = 1;
        
        return job;
    }
    
    Job* CreateChildJob(Job* parent, JobFunction function)
    {
        Job* job = new Job();
        job->parent = parent;
        job->function = function;
        job->args = nullptr;
        job->unfinishedJobs = 1;
        //parent->unfinishedJobs.fetch_add(1);
        parent->unfinishedJobs++;
        
        return job;
    }
    
    struct ParralelForJobData
    {
        int* array = nullptr;
        int count = 0;
        void (*function)(int*, int);
    };
    
    void ParallelForJobFunction(Job* job, void *args)
    {
        ParralelForJobData* data = static_cast<ParralelForJobData*>(args);
        if (data->count > 256)
        {
            int leftCount = data->count / 2;
            ParralelForJobData* leftData = new ParralelForJobData;
            leftData->array = data->array;
            leftData->count = leftCount;
            leftData->function = data->function;
            Job* leftJob = CreateChildJob(job, ParallelForJobFunction);
            leftJob->args = static_cast<void*>(leftData);
            JobSheduler::getInstance().AddJob(leftJob);
            
            int rightCount = data->count - leftCount;
            ParralelForJobData* rightData = new ParralelForJobData;
            rightData->array = data->array + leftCount;
            rightData->count = rightCount;
            rightData->function = data->function;
            Job* rightJob = CreateChildJob(job, ParallelForJobFunction);
            rightJob->args = static_cast<void*>(rightData);
            JobSheduler::getInstance().AddJob(rightJob);
        }
        else
        {
            (data->function)(data->array, data->count);
        }
    }
    
    Job* ParallelFor(ParallelFunction function, int* array, int count)
    {
        Job* job = CreateJob(ParallelForJobFunction);
        ParralelForJobData* data = new ParralelForJobData;
        data->array = array;
        data->count = count;
        data->function = function;
        job->args = static_cast<void*>(data);
        
        return job;
    }
    
    void Finish(Job* job)
    {
        //job->unfinishedJobs.fetch_sub(1);
        job->unfinishedJobs--;
        if (job->unfinishedJobs == 0)
        {
            if (job->parent)
                Finish(job->parent);
        }
    }
}
    
}


