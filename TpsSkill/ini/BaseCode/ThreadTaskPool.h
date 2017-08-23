/*
 * This file is part of `et engine`
 * Copyright 2009-2012 by Sergey Reznik
 * Please, do not modify contents without approval.
 *
 */

#pragma once

#include "ThreadTask.h"
#include "CriticalSection.h"

//namespace et
//{
	class ThreadTaskPool
	{
	public:
		ThreadTaskPool();
		~ThreadTaskPool();
		void update(float t);

		void addTask(ThreadTask* t, float delay = 0.0f);

	private:
		CriticalSection _csModifying;
		ThreadTaskList _tasks;
		ThreadTaskList _tasksToAdd;
		float _lastTime;
	};


//}