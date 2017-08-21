//http://blog.csdn.net/legend050709/article/details/39890997
//（5）字节序测试函数：
//不同cpu平台上字节序通常也不一样，下面写个简单的C程序，它可以测试不同平台上的字节序。
#include <stdio.h>
#include <netinet/in.h>
int main()
{
	int i_num = 0x12345678;
	printf("---<%p> 0x%x---\n", &i_num, i_num);
	printf("[0]:0x%x\n", *((char *)&i_num + 0));
	printf("[1]:0x%x\n", *((char *)&i_num + 1));
	printf("[2]:0x%x\n", *((char *)&i_num + 2));
	printf("[3]:0x%x\n", *((char *)&i_num + 3));

	i_num = htonl(i_num);
	printf("---<%p> 0x%x---\n", &i_num, i_num);
	printf("[0]:0x%x\n", *((char *)&i_num + 0));
	printf("[1]:0x%x\n", *((char *)&i_num + 1));
	printf("[2]:0x%x\n", *((char *)&i_num + 2));
	printf("[3]:0x%x\n", *((char *)&i_num + 3));

	return 0;
}  
