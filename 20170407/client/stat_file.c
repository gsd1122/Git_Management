#include "head.h"

int stat_file(char *filename)
{
	DIR *dir;
	dir=opendir(getcwd(NULL,0));
	struct stat s;
	struct dirent *d;
	long file_size=0;
	while((d=readdir(dir))!=NULL)
	{
		stat(d->d_name,&s);
		if(0==strcmp(d->d_name,filename))
		{
			file_size=s.st_size;
			closedir(dir);
			return file_size;
		}if(NULL==d){
				closedir(dir);
				return 0;
			}
	}
}
