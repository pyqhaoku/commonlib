#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pdate.h"
#include "table.h"
#include "pstring.h"

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期函数测试 初始日期2018-01-01 AddDays 1天1万次 2天5000次 4天2500次 5天2000次 8天1250次 10天1000次 16天625次 625天16次 1000天10次 5000天2次  1月 2月 10 11 12 13 14月 100月 1年 2年 3年 4年 5年
 * 生成csv文件查看结果
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int main()
{
	char *testlist[] = {"1day", "2day", "4day", "5day", "8day", "10day", "16day", "625day", "1000day", "5000day", "1month", "2month", "10month", "11month", "12month", "13month", "14month", "100month", "1year", "2year", "3year", "4year", "5year"};

	int i, size = sizeof(testlist) / sizeof(char *);
	pt_table *pt = createTable(12000, size + 1);

	for( i = 0; i < size; i++)
	{
		setTableValue(pt, 0, i, testlist[i]);
		pdate spd;
		dateInit(&spd, 2018, 1, 1);

		printf("start %s test\n", testlist[i]);
		int j = 10000;
		int d = 0;
		char *temp = NULL;
		char value[32];
		d = strtol(testlist[i], &temp, 10);
		if(d <= 0) d = 1;

		snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
		setTableValue(pt, 1, i, value);

		int index = 1;
		while(j > 0)
		{
			if(strcasecmp(temp, "day") == 0)
			{
				j -= d;
				dateSubDays(&spd, d);
				int k = 0;
				while(++k < d)
				{
					setTableValue(pt, index + k, i, "");
				}

				index += k;
				//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
				snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
				setTableValue(pt, index, i, value);
			}
			else if(strcasecmp(temp, "month") == 0)
			{
				int rc = dateSubMonths(&spd, d);
				int k = 0;
				j -= rc;
				while(++k < rc)
				{
					setTableValue(pt, index + k, i, "");
				}

				index += k;
				//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
				snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
				setTableValue(pt, index, i, value);
			}
			else if(strcasecmp(temp, "year") == 0)
			{
				int rc = dateSubYears(&spd, d);
				int k = 0;
				j -= rc;
				while(++k < rc)
				{
					setTableValue(pt, index + k, i, "");
				}

				index += k;
				//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
				snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
				setTableValue(pt, index, i, value);
			}
		}
	}

	saveTableCsv(pt, "datesubtest.csv");
	freeTable(pt);

	pt = createTable(12000, size + 1);

	for( i = 0; i < size; i++)
	{
		setTableValue(pt, 0, i, testlist[i]);
		pdate spd;
		dateInit(&spd, 1970, 1, 1);

		int j = 10000;
		int d = 0;
		char *temp = NULL;
		char value[32];
		d = strtol(testlist[i], &temp, 10);
		if(d <= 0) d = 1;

		snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
		//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
		setTableValue(pt, 1, i, value);

		int index = 1;
		while(j > 0)
		{
			if(strcasecmp(temp, "day") == 0)
			{
				j -= d;
				dateAddDays(&spd, d);
				int k = 0;
				while(++k < d)
				{
					setTableValue(pt, index + k, i, "");
				}

				index += k;
				//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
				snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
				setTableValue(pt, index, i, value);
			}
			else if(strcasecmp(temp, "month") == 0)
			{
				int rc = dateAddMonths(&spd, d);
				int k = 0;
				j -= rc;
				while(++k < rc)
				{
					setTableValue(pt, index + k, i, "");
				}

				index += k;
				//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
				snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
				setTableValue(pt, index, i, value);
			}
			else if(strcasecmp(temp, "year") == 0)
			{
				int rc = dateAddYears(&spd, d);
				int k = 0;
				j -= rc;
				while(++k < rc)
				{
					setTableValue(pt, index + k, i, "");
				}

				index += k;
				//snprintf(value, sizeof(value), "%04d-%02d-%02d", spd.year, spd.month, spd.day);
				snprintf(value, sizeof(value), "%04d-%02d-%02d(week:%d-%dydays:%d)", spd.year, spd.month, spd.day, spd.weeks, spd.wday, spd.ydays);
				setTableValue(pt, index, i, value);
			}
		}
	}

	saveTableCsv(pt, "dateaddtest.csv");
	freeTable(pt);

	return 0;
}
