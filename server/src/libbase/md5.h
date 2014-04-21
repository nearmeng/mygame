#ifndef __SAFE_LOCK_MD5_H__
#define __SAFE_LOCK_MD5_H__

#include <string>

using namespace std;

#define  __alpha

#ifdef __alpha
typedef unsigned int uint32_t;
#else
//md5算法的实现里面都是这么定义的
//如此32位机器跟64位机器来加密值会不同
//在前面定义了__alpha
typedef unsigned long uint32_t;
#endif

//只需要暴露一个接口就行了，其他看不出有什么用，声明全部放到cpp
std::string calc_md5_string(unsigned char *input, unsigned int inlen);

#endif /* !MD5_H */

