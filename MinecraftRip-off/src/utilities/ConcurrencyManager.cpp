#include "ConcurrencyManager.h"

ConcurrencyManager::ConcurrencyManager()
	:
	threadPool{ (std::thread::hardware_concurrency() > 1) ? (std::thread::hardware_concurrency() - 1) : 1 }
{

}

void ConcurrencyManager::addReadTask(std::function<void()> task)
{
	readTasks.push_back(std::move(task));
}

void ConcurrencyManager::addWriteTask(std::function<void()> task)
{
	writeTasks.push_back(std::move(task));
}

void ConcurrencyManager::addChunkMeshTask(std::function<void()> task)
{
	chunkMeshTasks.push_back(std::move(task));
}

void ConcurrencyManager::addSetBlockTask(std::function<void()> task)
{
	setBlockTasks.push_back(std::move(task));
}

void ConcurrencyManager::updateTasks()
{
	for (auto it = futures.begin(); it != futures.end();)
	{
		if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			it->get();
			it = futures.erase(it);
		}
		else
		{
			it = std::next(it);
		}
	}

	if (futures.empty())
	{
		if (!writeTasks.empty())
		{
			for (auto it = writeTasks.begin(); it != writeTasks.end();)
			{
				futures.emplace_back(threadPool.enqueue(std::move(*it)));
				it = writeTasks.erase(it);
			}
		}
		else if (!readTasks.empty())
		{
			for (auto it = readTasks.begin(); it != readTasks.end();)
			{
				futures.emplace_back(threadPool.enqueue(std::move(*it)));
				it = readTasks.erase(it);
			}
		}
		else if (!chunkMeshTasks.empty())
		{
			for (auto it = chunkMeshTasks.begin(); it != chunkMeshTasks.end();)
			{
				futures.emplace_back(threadPool.enqueue(std::move(*it)));
				it = chunkMeshTasks.erase(it);
			}
		}
		else if (!setBlockTasks.empty())
		{
			for (auto it = setBlockTasks.begin(); it != setBlockTasks.end();)
			{
				futures.emplace_back(threadPool.enqueue(std::move(*it)));
				it = setBlockTasks.erase(it);
			}
		}
	}
}
