#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <dirent.h>
/*
	read_All_dir: 假设定义一个目录的大小为 
		该目录下面以及目录的目录下面,......,
	  	的普通文件的大小之和,请你写一个函数用来求一个目录的大小
	@pathname :传入的文件目录路径
	返回值:
		目录的大小
*/
long long  read_All_dir(const char * pathname)
{
	//1.打开目录
	DIR * fd = opendir(pathname);
	if(fd == NULL)
	{
		perror("opendir error");
		return -1;
	}
	//printf("opendir %s success!\n",pathname);

	//2.读取目录项
	struct dirent * p = NULL;
	char  workdirctory[512];
	//用来保存文件信息的结构体
	struct stat st ;
	//用来记录目录总大小
	long long total_size = 0;
	chdir(pathname);
	char path[256] = {0};
	getcwd(path,256);
	while(p = readdir(fd))
	{
		//忽略 文件 . ..
		if(strcmp(p->d_name,".") == 0 ||strcmp(p->d_name,"..") == 0)
		{
			continue;
		}
		sprintf(workdirctory,"%s/%s",path,p->d_name);
        // 获取文件信息
        if (stat(workdirctory, &st) == -1) 
		{
            perror("stat error");
            continue;
        }

		// 判断是否为目录
		if (S_ISDIR(st.st_mode)) 
		{
			// 递归进入子目录
			total_size += read_All_dir(workdirctory);
		} 
		else if (S_ISREG(st.st_mode)) 
		{
			// 普通文件，累加其大小
			total_size += st.st_size;
		}

	}


	//3.关闭目录
	closedir(fd);
	return total_size;

}

int main(int argc , char * argv[])
{
	if(argc != 2)
	{
		perror("Input error");
		return -1;
	}
	long long total_size = read_All_dir(argv[1]);
	printf("%s目录的总大小为:%lld\n",argv[1],total_size);
	return 0;
}

