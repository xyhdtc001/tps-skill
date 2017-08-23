#pragma once

template <class t_class>
class Singleton
{
public:
	static t_class& instance()
	{
		static t_class _instance;
		return _instance;
	}

protected: 
	Singleton() { };
	Singleton(const Singleton&) { };
	Singleton& operator = (const Singleton&) { };
};

#define ET_SINGLETON_CONSTRUCTORS(t) private:	\
	t() { }										\
	t(const t&) { }								\
	t& operator = (const t&) { return *this; }	\
	friend class Singleton<t>;

#define ET_DENY_COPY(t) private:				\
	t(const t&) { }								\
	t& operator = (const t&) { return *this; }

#define ET_SINGLETON_COPY_DENY(t) private:		\
	ET_DENY_COPY(t)								\
	friend class Singleton<t>;
	