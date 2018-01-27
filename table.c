/**
 * @file table.c
 * @brief 表格格式化输出 
 * @author pyq mail:pyqhaoku@163.com
 * @version v1.0
 * @date 2017-12-09
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <table.h>
#include <pstring.h>
/* ----------------------------------------------------------------------------*/
/**
 * @brief 新建一个table
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
pt_table * createTable(int rowsize, int colsize)
{
	pt_table *pt = malloc(sizeof(pt_table));
	memset(pt, 0, sizeof(pt_table));

	pt->rowsize = rowsize;
	pt->colsize = colsize;

	pt->rownum = rowsize;
	pt->colnum = colsize;

	int irow, icol;

	pt->rows = malloc(rowsize * sizeof(pt_row));
	pt->cols = malloc(colsize * sizeof(pt_col));

	memset(pt->rows, 0, sizeof(pt_row) * rowsize);
	memset(pt->cols, 0, colsize * sizeof(pt_col));

	for( irow = 0; irow < rowsize; irow++)
	{
		pt->rows[irow].cells = malloc(colsize * sizeof(pt_cell *));
		memset(pt->rows[irow].cells, 0, colsize * sizeof(pt_cell *));
		pt->rows[irow].cellsize = colsize;
		// format
	}
	for( icol = 0; icol < colsize; icol++)
	{
		pt->cols[icol].cells = malloc(rowsize * sizeof(pt_cell *));
		memset(pt->cols[icol].cells, 0, rowsize * sizeof(pt_cell *));
		pt->cols[icol].cellsize = rowsize;
	}
	return pt;
}

int setHeadTitle(pt_table *pt, colnameinfo *ci, int cisize)
{
	int icol;
	pt_row *pr = &(pt->rows[0]);
	for(icol = 0; icol < pt->colnum; icol ++)
	{
		int j;
		for(j = 0; j < cisize; j++)
		{
			char *value = pr->cells[icol]->value;
			if(strncmp(value, "F_", 2) == 0)
			{
				value += 2;
			}
			if(strcmp(value, ci[j].colname) == 0)
			{
				setTableValue(pt, 0, icol, ci[j].cnname);
			}
		}
	}
	return 0;
}

int showTable(pt_table *pt)
{
	int startindex = 1, rows = 10, pagenum = 0;
	int lastpage = (pt->rownum + rows - 2) / rows;
	startindex = pagenum * rows + 1;
	showTableRows(pt, startindex, rows);
	if(lastpage == 1)
	{
		return 0;
	}

	// echo off
	system("stty -echo");
	system("stty cbreak");
	//system("stty raw");
	printp("page:%d/%d\tinput n next page; p prev page; q quit; a last all;", pagenum + 1, lastpage);
	fflush(stdout);
	//noecho();
	while(1)
	{
		char ch = getchar();
		if(ch == 'q')
		{
			printf("\n");
			break;
		}
		else if(ch == 'a')
		{
			printf("\n");
			showTableRows(pt, startindex, pt->rownum - startindex);
			printp("page:%d/%d\tinput n next page; p prev page; q quit; a last all;", pagenum + 1, lastpage);
			fflush(stdout);
			break;
		}
		else if(ch == 'p')
		{
			if(pagenum > 0)
			{
				printf("\n");
				pagenum --;
				startindex = pagenum * rows + 1;
				showTableRows(pt, startindex, rows);
				printp("page:%d/%d\tinput n next page; p prev page; q quit; a last all;", pagenum + 1, lastpage);
			}
			else
			{
				printf("\n");
				printp("first page\n");
			}
		}
		else if(ch == 'n')
		{
			if(pagenum + 1 < lastpage)
			{
				printf("\n");
				pagenum ++;
				startindex = pagenum * rows + 1;
				showTableRows(pt, startindex, rows);
				printp("page:%d/%d\tinput n next page; p prev page; q quit; a last all;", pagenum + 1, lastpage);
			}
			else
			{
				printf("\n");
				printp("last page\n");
			}
		}
	}
	//echo();
	//echo on
	system("stty echo");
	system("stty -cbreak");
	//system("stty cooked");

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 输出表中第startindex行之后的rows行记录
 *
 * @param pt
 * @param startindex
 * @param rows
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int showTableRows(pt_table *pt, int startindex, int rows)
{
	char *buf = NULL;
	int buf_size = 1;

	int i;
	for( i = 0; i < pt->colnum; i++)
	{
		buf_size += pt->cols[i].maxlength + 3; // | value |  
	}

	// 防止中文
	buf_size *= 10;

	buf = malloc(buf_size);
	memset(buf, '-', buf_size);
	buf[buf_size/10] = 0;
	printp("%s\n\r", buf);
	//
	for( i = 0 ; i < 1; i++)
	{
		int j = 0;
		pt_row irow = pt->rows[i];

		snprintf(buf, buf_size, "| ");
		for( j = 0; j < irow.count ; j++)
		{
			pt_cell *p = irow.cells[j];
			int len = utf8_strlen(p ? p->value : "");
			int maxlen = pt->cols[p->icol].maxlength;

			int left = (maxlen - len) / 2;
			int right = maxlen - len - left;

			while(left--) strcat(buf, " ");
			strcat(buf, p ? p->value : "");
			while(right--) strcat(buf, " ");
			strcat(buf, " | ");

		}

		printp("%s\n\r", buf);

		memset(buf, '-', buf_size);
		buf[buf_size/10] = 0;
		printp("%s", buf);
		printp("\n\r");
	}

	//refresh();
	int lastrow = pt->rownum > (startindex + rows) ? (startindex + rows) : pt->rownum;
	for( i = startindex ; i < lastrow; i++)
	{
		int j = 0;
		pt_row irow = pt->rows[i];

		snprintf(buf, buf_size, "| ");
		for( j = 0; j < irow.count ; j++)
		{
			pt_cell *p = irow.cells[j];
			int len = utf8_strlen(p ? p->value : "");
			int maxlen = pt->cols[p->icol].maxlength;

			int left = (maxlen - len) / 2;
			int right = maxlen - len - left;

			while(left--) strcat(buf, " ");
			strcat(buf, p ? p->value : "");
			while(right--) strcat(buf, " ");
			strcat(buf, " | ");

		}

		printp("%s\n\r", buf);

		if(i == lastrow - 1 )
		{
			memset(buf, '-', buf_size);
			buf[buf_size/10] = 0;
			printp("%s", buf);
		}
		printp("\n\r");
		//refresh();
		//sleep(1);
	}

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 添加一行记录 一般不用
 *
 * @param pt
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int addTableRow(pt_table *pt)
{
	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 添加一列记录 一般不用
 *
 * @param pt
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int addTableCol(pt_table *pt)
{
	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 设置某个位置的value
 *
 * @param pt
 * @param irow
 * @param icol
 * @param value
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int setTableValue(pt_table *pt, int irow, int icol, char *value)
{
	if(irow < 0 || irow >= pt->rownum || icol < 0 || icol >= pt->colnum)
	{
		return -1;
	}

	// log
	//printf("pt[%d][%d]=|%s|\n", irow, icol, value);
	pt_col *pc = &(pt->cols[icol]);

	// ---
	if(pc->cells == NULL)
	{
		pc->cells = malloc(pt->rowsize * sizeof(pt_cell *));
		memset(pc->cells, 0, pt->rowsize * sizeof(pt_cell *));
		pc->count = 0;
		pc->cellsize = pt->rowsize;
	}
	pt_cell *pce = pc->cells[irow];

	if(pce == NULL)
	{
		pc->cells[irow] = malloc(sizeof(pt_cell));
		pce = pc->cells[irow];
		memset(pce, 0, sizeof(pt_cell));

		pce->irow = irow;
		pce->icol = icol;

		pt->rows[irow].cells[icol] = pce;
		pc->count += 1;
		pt->rows[irow].count += 1;

		// check pt_row->cells
	}

	// check value
	if(pce->value != NULL)
	{
		free(pce->value);
	}

	pce->value = strcpy_p(value);
	int len = utf8_strlen(pce->value);
	if(len > pc->maxlength)
	{
		pc->maxlength = len;
	}

	return 0;
}

int freeTable(pt_table *pt)
{
	// free col and cell
	int icol,irow;
	for( icol = 0; icol < pt->colsize; icol++)
	{
		pt_col *pc = &(pt->cols[icol]);
		for( irow = 0; irow < pt->rowsize; irow++)
		{
			pt_cell *pce = pc->cells[irow];
			if(pce == NULL) continue;

			if(pce->value != NULL)
			{
				free(pce->value);
				pce->value = NULL;
			}

			if(pce->prev != NULL)
			{
				pce->prev = NULL;
			}

			if(pce->next != NULL)
			{
				pce->next = NULL;
			}

			free(pce);
		}

		free(pc->cells);
		pc->cells = NULL;
	}

	// free row
	for( irow = 0; irow < pt->rowsize; irow++)
	{
		pt_row *pr = &(pt->rows[irow]);
		if(pr == NULL)
		{
			continue;
		}
		free(pr->cells);
		pr->cells = NULL;
	}

	// free table
	free(pt->rows);
	free(pt->cols);
	free(pt);

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief csv格式化 当csv中有, " 等特殊符号时 两边加 " 字符串中的" 变为 ""
 *
 * @param value
 * @param result
 * @param size
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int csvFormat(char *value, char *result, int size)
{
	if(value == NULL || result == NULL || size <= 0)
	{
		//_WriteErrorLog("parameter error\n");
		return -1;
	}

	int l = strlen(value);
	int index = 0, count = l, i = 0;
	if(l > size)
	{
		//_WriteErrorLog("result array is small\n" );
		return -1;
	}

	int flag = 0;
	for( i = 0; i < l ; i++)
	{
		if(value[i] == '"' || value[i] == ',')
		{
			flag = 1;
			break;
		}
	}

	if(flag == 1)
	{
		count += 2;
		result[index++] = '"';
	}

	if(count > size )
	{
		return -1;
	}

	for(i= 0; i< l ; i++)
	{
		if(value[i] == '"')
		{
			count++;
			if(count > size)
			{
				//_WriteErrorLog("result array is small\n" );
				return -1;
			}
			result[index++] = value[i];
			result[index++] = value[i];
		}
		else
		{
			result[index++] = value[i];
		}
	}

	if(flag == 1)
	{
		result[index++] = '"';
	}

	result[index++] = 0;
	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 将table保存为csv文件
 *
 * @param pt
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int saveTableCsv(pt_table *pt, char *filename)
{
	char *buf = NULL;
	int buf_size = 1;

	int i;
	for( i = 0; i < pt->colnum; i++)
	{
		buf_size += pt->cols[i].maxlength + 3; // | value |  
	}

	buf_size *= 3;

	buf = malloc(buf_size);

	FILE *fd = fopen(filename, "w");
	// --utf8 code in windows
	fprintf(fd, "\xef\xbb\xbf");
	int lastrow = pt->rownum;
	for( i = 0; i < lastrow; i++)
	{
		int j = 0, index = 0;
		memset(buf, 0, buf_size);
		pt_row irow = pt->rows[i];
		for( j = 0; j < irow.count; j++)
		{
			pt_cell *p = irow.cells[j];
			if( p != NULL)
				csvFormat(p->value, buf + index, buf_size - index);
			strcat(buf, ",");
			index = strlen(buf);
		}

		fprintf(fd, "%s\n", buf);
	}

	fclose(fd);

	return 0;
}
