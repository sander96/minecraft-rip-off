#ifndef CONCURRENCYMANAGER_H
#define CONCURRENCYMANAGER_H

#include <list>
#include "ThreadPool.h"
#include <future>
#include "../chunk/ChunkMesh.h"
#include "../chunk/Chunk.h"

class ConcurrencyManager
{
public:
	ConcurrencyManager();

	void addReadTask(std::function<void()> task);
	void addWriteTask(std::function<void()> task);
	void addChunkMeshTask(std::function<void()> task);

	void updateTasks();

	bool empty() { return futures.empty() && readTasks.empty() && writeTasks.empty() && chunkMeshTasks.empty(); }


private:
	ThreadPool threadPool;

	std::list<std::function<void()>> readTasks;
	std::list<std::function<void()>> writeTasks;
	std::list<std::function<void()>> chunkMeshTasks;

	std::list<std::future<void>> futures;
};

#endif