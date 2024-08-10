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
	get_mode : 获取用户/用户组/其他的用户权限
	@st :保存一个文件的状态或属性信息的结构体
	@type :用户的类型
	返回值:
			无
*/
void get_mode(struct stat st ,int type1,int type2,int type3)
{
	if(st.st_mode & type1)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}

	if(st.st_mode & type2)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}

	if(st.st_mode & type3)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}


}
void Print_file_info(const char * filepath)
{
	
	struct stat st;//用来保存文件属性信息
	int r = stat(filepath,&st);
	if(r == -1)
	{
		perror("stat error");
		return ;
	}
	if(stat(filepath,&st) != 0)
	{
		perror("获取信息失败 ");
		return ;
	}

	//文件类型
	if(S_ISREG(st.st_mode))
	{
		//普通文件
		printf("-");
	}
	else if(S_ISDIR(st.st_mode))
	{
		//目录文件
		printf("d");
	}
	else if(S_ISLNK(st.st_mode))
	{
		//链接文件
		printf("l");
	}
	else if(S_ISFIFO(st.st_mode))
	{
		//管道文件
		printf("p");
	}
	else
	{
		//其他文件
		printf("o");
	}

	//----文件的权限----
	
	//1.用户权限
	get_mode(st ,S_IRUSR,S_IWUSR,S_IXUSR);
	//2.用户组权限
	get_mode(st ,S_IRGRP,S_IWGRP,S_IXGRP );
	//3.其他用户权限
	get_mode(st ,S_IROTH,S_IWOTH,S_IXOTH);

	//----硬链接数量----
	int count = st.st_nlink;
	printf("%d\t",count);

	//----文件用户----
	int uid = st.st_uid;
	struct passwd * pw = getpwuid(uid);
	printf("%s\t",pw->pw_name);

	//----用户所在用户组----
	int gid = st.st_gid;
	struct group *gr =	getgrgid(gid);
	printf("%s\t",gr->gr_name);

	//----文件大小----
	int size = st.st_size;
	printf("%d\t",size);

	//----文件的最后修改时间----
	char * p = ctime((const time_t * )&st.st_mtim);
	p[strlen(p) - 1] = '\0';
	printf("%s\t",p);

	//----文件名----
	printf("%s\n",filepath);


}
int main(int argc, char * argv[])
{
	//1.打开一个目录文件
	DIR * pf = opendir(argv[1]);
	
	//打印错误信息
	if(pf == NULL)
	{
		perror("opendir error");
		return -1;
	}
	printf("opendir %s success!\n",argv[1]);
	//2.读取目录项
	
	//改变路径
	chdir(argv[1]);
	char path[256] = {0};
	getcwd(path,256);
	
	//保存了目录项的属性d_ino,d_name
	struct dirent * p = NULL;

	//设置一个数组保存绝对路径和文件名
	char pathname[512];
	//3.传入打印函数中,打印出文件的信息
	while(p = readdir(pf))
	{
		// 忽略 "." 和 ".."
		if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0)
		{
			continue;
		}
		sprintf(pathname,"%s/%s",path,p->d_name);
		Print_file_info(pathname);
	}

	//4.关闭目录文件
	closedir(pf);
	return 0;
}
