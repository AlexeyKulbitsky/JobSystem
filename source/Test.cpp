#include "Job.h"
#include "Threads.h"
#include "JobScheduler.h"
#include "Timer.h"

#include "Test.h"

#include <atomic>
#include <iostream>

#include <map>
#include <cmath>

const int iterations = 100000;
int array[iterations];

std::map<std::thread::id, int> executionMap;

void ParallelEmptyFunc(int* array, int count)
{
    for (int i = 0; i < count; ++i)
    {
        const auto r = std::rand();
        array[i] = 1 * r;
        array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
        array[i] /= 4;
        array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
        array[i] /= 4;
        array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
        array[i] /= 4;
        array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
        array[i] /= 4;
    }
    
    //executionMap[std::this_thread::get_id()] += count;
}



void TestJobs()
{
	int32_t count = sh::jobs::GetHardwareThreadsCount();
	std::cout << "Threads count: " << count - 1 << std::endl;
	Timer timer;

	// Parallel
    sh::jobs::JobSheduler::getInstance().Init();
    const size_t steps = 1000;
    double time = 0.0;
    for (size_t i = 0; i < steps; ++i)
    {
        timer.Reset();
        timer.Start();
        
        auto job = sh::jobs::ParallelFor(ParallelEmptyFunc, array, iterations);
        sh::jobs::JobSheduler::getInstance().AddJob(job);
        sh::jobs::JobSheduler::getInstance().WaitForJob(job);
        
        timer.Stop();
        time += timer.GetTimeMS();
    }
	
	std::cout << "Time parallel: " << time / steps << std::endl;
	sh::jobs::JobSheduler::getInstance().Clear();
    
    int sum = 0;
    for (auto& it : executionMap)
    {
        std::cout << "[THREAD] " << it.second << std::endl;
        sum += it.second;
    }
    std::cout << "[SUM] " << sum << std::endl;
	// Linear
    time = 0.0;
    for (size_t i = 0; i < steps; ++i)
    {
        timer.Reset();
        timer.Start();
        for (int i = 0; i < iterations; ++i)
        {
            const auto r = std::rand();
            array[i] = 1 * r;
            array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
            array[i] /= 4;
            array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
            array[i] /= 4;
            array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
            array[i] /= 4;
            array[i] *= 2 + static_cast<int>(std::sin(std::rand()) * 1236.572346f);
            array[i] /= 4;
        }
        timer.Stop();
        time += timer.GetTimeMS();
    }
    
	std::cout << "Time linear: " << time / steps << std::endl;
}
