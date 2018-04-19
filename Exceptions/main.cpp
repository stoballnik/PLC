// В работе используется статья https://habrahabr.ru/post/208006/

#include "setjmp.h"
#include <iostream>
#include <assert.h>
#include <string.h>

struct base_node {
	base_node();
	virtual ~base_node();
	void unreg();
	base_node  *prev_;
};

struct jmp_node {
	jmp_node();
	~jmp_node();
	jmp_buf buf_;
	jmp_node *prev_;
	base_node* root_objs_;
};

jmp_node *root_slice_ = NULL;
jmp_node::jmp_node()
{
	root_objs_ = NULL;
	prev_ = root_slice_;
	root_slice_ = this;
}
jmp_node::~jmp_node()
{
	root_slice_ = prev_;
}

base_node::base_node()
{
	if (root_slice_)
	{
		prev_ = root_slice_->root_objs_;
		root_slice_->root_objs_ = this;
	}
}
base_node::~base_node()
{
	unreg();
}
void base_node::unreg()
{
	if (root_slice_ && prev_)
	{
		root_slice_->root_objs_ = prev_;
		prev_ = NULL;
	}
}

int pop_slice()
{
	jmp_node *sl = root_slice_;
	assert(sl);
	root_slice_ = sl->prev_;
	return 0;
}
int throw_slice(const char *str)
{
	if (!str)
		return -1;
	jmp_node *sl = root_slice_;
	base_node *obj = root_slice_->root_objs_;
	while (obj)
	{
		base_node *tmp = obj;
		obj = obj->prev_;
		tmp->~base_node();
	}
	pop_slice();
	longjmp(sl->buf_, int(str));
	return 0;
}

#define TRY_BLOCK { \
  jmp_node __sl; \
  const char *__exc = (const char *)setjmp (__sl.buf_); \
  if (!__exc) {

#define CATCH_BLOCK(exc)  \
  } else \
	if (strcmp(__exc, exc) == 0) {

#define END_BLOCK  \
    } \
	else { \
	throw_slice(__exc); \
	}\
  }

#define THROW_IN_BLOCK(exc)  \
  throw_slice (exc);


#define RETHROW_IN_BLOCK  \
  throw_slice (__exc);


class _A: public base_node {
public:
	_A() :val_(0) { std::cout << "A::A(" << val_ <<")\n"; }
	_A(int i) :val_(i) { std::cout << "A::A(" << val_ << ")\n"; }
	virtual ~_A() { std::cout << "A::~A(" << val_ << ")\n"; }
private:
	int val_;
};


int main() {
	_A a(1);
	TRY_BLOCK{
		TRY_BLOCK{
			_A b(2);
		THROW_IN_BLOCK("error\n");
		std::cout << "notreached\n";
		}
		CATCH_BLOCK("error\n") {
			std::cout << __exc;
			RETHROW_IN_BLOCK
		}
		END_BLOCK;
	}
	CATCH_BLOCK("error\n") {
		std::cout << __exc;
	}
	END_BLOCK;
	return 0;
}