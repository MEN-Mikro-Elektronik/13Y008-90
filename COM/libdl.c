/* libdl.c */
#include <stdio.h>

#define unused_arg(_t, _a) _t _a __attribute__ ((unused))

void* dlopen (unused_arg(char const*, filename),
              unused_arg(int, flag))
{
  return (void*) 0;
}

const char* dlerror (void)
{
  return 0;
}

void* dlsym (unused_arg(void*, handle),
             unused_arg(char*, symbol))
{
  return (void*) 0;
}

int dlclose (unused_arg(void*, handle))
{
  return 0;
}

void *
__libc_dlopen (unused_arg(const char, *__name))
{
	return NULL;
}

void *
__libc_dlsym (unused_arg(void *,__map), unused_arg(const char *,__name))
{
	return NULL;
}

int
__libc_dlclose (unused_arg(void *,__map))
{
	return 1;
}

