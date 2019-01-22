#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

typedef struct _tar_header
{                              /* byte offset */
	char name[100];               /*   0 */
	char mode[8];                 /* 100 */
	char uid[8];                  /* 108 */
	char gid[8];                  /* 116 */
	char size[12];                /* 124 */
	char mtime[12];               /* 136 */
	char chksum[8];               /* 148 */
	char typeflag;                /* 156 */
	char linkname[100];           /* 157 */
	char magic[6];                /* 257 */
	char version[2];              /* 263 */
	char uname[32];               /* 265 */
	char gname[32];               /* 297 */
	char devmajor[8];             /* 329 */
	char devminor[8];             /* 337 */
	char prefix[155];             /* 345 */
	char padding[12];             /* 500 */
}tar_header;

int GetSize(char size[12])
{
	int res = 0;
	int i = 0;
	for(i = 0 ; i < 12 ; i++)
	{
		if(size[i] == 0) break;
		res = res * 8 + size[i] -'0';
	}

	return res;
}

char *GetTarFile(char *tarfile, char *targetfile, int *size)
{
	char *targetContent = NULL;
	gzFile test = gzopen(tarfile,"r");
	int offset = 0;
	*size = 0;

	if(test == NULL)
	{
		return NULL;
	}

	char c[512];
	while(1)
	{
		memset(c,0,sizeof(c));
		int ret = gzread(test,c,512);
		offset += 512;

		if(ret <= 0)
		{
			break;
		}
		tar_header *head = (tar_header *) c;

		if(strncmp(head->magic, "ustar ", 6) != 0)
		{
			break;
		}
		
		int csize = GetSize(head->size);
		int bsize = ((csize + 511) / 512) * 512;
		int len = strlen(targetfile);
		if(len > 100) len = 100;
		offset += bsize;

		if(strncmp(head->name,targetfile,len) == 0)
		{
			targetContent = (char *)malloc(bsize);
			int rc = gzread(test, targetContent, bsize);
			if(rc < 0)
			{
				free(targetContent);
				*size = 0;
				return NULL;
			}
			*size = csize;
		}
		else
		{
			gzseek(test, offset, SEEK_SET);
		}

	}
	gzclose(test);

	return targetContent;
}

