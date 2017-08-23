#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

class NonCopyable
{
public:
	NonCopyable() {}

private:
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&);
};

#endif
