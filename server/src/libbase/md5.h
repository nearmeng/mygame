#ifndef __SAFE_LOCK_MD5_H__
#define __SAFE_LOCK_MD5_H__

#include <string>

using namespace std;

#define  __alpha

#ifdef __alpha
typedef unsigned int uint32_t;
#else
//md5�㷨��ʵ�����涼����ô�����
//���32λ������64λ����������ֵ�᲻ͬ
//��ǰ�涨����__alpha
typedef unsigned long uint32_t;
#endif

//ֻ��Ҫ��¶һ���ӿھ����ˣ�������������ʲô�ã�����ȫ���ŵ�cpp
std::string calc_md5_string(unsigned char *input, unsigned int inlen);

#endif /* !MD5_H */

