/*
 * This file is part of `et engine`
 * Copyright 2009-2012 by Sergey Reznik
 * Please, do not modify contents without approval.
 *
 */

#pragma once

#include <list>

//namespace et
//{
	class ThreadTask
	{
	protected:
		ThreadTask() : _executionTime(0.0f) { }
		virtual ~ThreadTask()	{ }

		virtual void execute() = 0;

	private:
		float executionTime() const 
			{ return _executionTime; }

		void setExecutionTime(float t)
			{ _executionTime = t; }

		friend class ThreadTaskPool;

	private:
		float _executionTime;
	};
	typedef std::list<ThreadTask*> ThreadTaskList;

	template <typename T>
	class ThreadTaskDeletion : public ThreadTask
	{
	public:
		ThreadTaskDeletion(T* obj) : _object(obj) { }

		void execute()
			{ delete _object; }

	private:
		T* _object;
	};
//}