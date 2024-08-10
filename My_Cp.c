#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
	my_cp:将file1中的内容复制到file2中
	@path1
	@path2
	返回值：
		-1 复制失败
		0 复制成功
*/
int my_cp(const char * path1,const char * path2)
{
	//1.先打开两个文件
	int fd1 = open(path1,O_RDWR);
	if(fd1 == -1)
	{
		perror("open file1 error");
		return -1;
	}
	int fd2 = open(path2,O_RDWR|O_CREAT|O_TRUNC, 0777);
	if(fd2 == -1)
	{
		perror("open file2 error");
		return -1;
	}

	//2.将file1中的内容复制到file2中



	while(1)
	{
		//读取file1中的内容
		char fbuf[20];
		int count = read(fd1,fbuf,20);
		if(count == -1)
		{
			perror("read file1 error");
			close(fd1);
			close(fd2);
			return -1;
		}		
		//file1读到了末尾，那么就退出循环
		if(count == 0)
		{
			close(fd1);
			close(fd2);
			break;
		}

		//将file1中读到的内容,写到file2中
		int res = write(fd2,fbuf,count);
		if(res == -1)
		{
			perror("write file2 error");
			close(fd1);
			close(fd2);
			return -1;
		}
		
	}

	//3.读写成功,关闭文件
	close(fd1);
	close(fd2);
	return 0;
	
}
int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		printf("Input error\n");
		return -1;
	}
	int res = my_cp(argv[1], argv[2]);
	if(res == -1)
	{
		printf("copy %s -> %s error\n",argv[1],argv[2]);
	}
	else
	{
		printf("copy %s -> %s success\n",argv[1],argv[2]);
	}
	return 0;
}
