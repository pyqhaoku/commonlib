#ifndef __TABLE_H_
#define __TABLE_H_

typedef struct pt_cell{
	int irow;
	int icol;
	char *value;
	struct pt_cell *prev;   // row - 1 
	struct pt_cell *next;   // row + 1
}pt_cell;

typedef struct pt_row{
	int count;
	int cellsize;           // cell指针数组大小  默认100
	pt_cell *first;
	pt_cell *last;
	pt_cell **cells;        // cell指针数组 定位使用
}pt_row;

typedef struct pt_col{
	int count;
	int cellsize;           // cell指针数组大小  默认100
	int maxlength;          // 该列中最长字符串的长度  输出时占用的长度依据
	pt_cell *first;
	pt_cell *last;
	pt_cell **cells;        // cell指针数组 定位使用
}pt_col;

typedef struct pt_table{
	int rownum;
	int colnum;
	int rowsize;
	int colsize;

	pt_row *rows;
	pt_col *cols;

	pt_row *colinfo;       // 第一行  列名信息
}pt_table;

// 列名转换
typedef struct colnameinfo{
	char *colname;
	char *cnname;
}colnameinfo;

// use printw scanw replace printf scanf
#ifndef printp
#define printp printf
#endif

#ifndef scanp
#define scanp scanf
#endif

pt_table * createTable(int rowsize, int colsize);
int showTable(pt_table *pt);
int showTableRows(pt_table *pt, int startindex, int rows);
int addTableRow(pt_table *pt);
int addTableCol(pt_table *pt);
int freeTable(pt_table *pt);

int setHeadTitle(pt_table *pt, colnameinfo *ci, int cisize);
int setTableValue(pt_table *pt, int irow, int icol, char *value);

int saveTableCsv(pt_table *pt, char *filename);

#endif
