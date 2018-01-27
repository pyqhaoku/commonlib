#include <stdio.h>

#include "pdate.h"

static int mdays[12]    =    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int arrydays[13] = {0, 31, 59, 90,120,151,181,212,243,273,304,334,365};

/* ----------------------------------------------------------------------------*/
/**
 * @brief 判断是否是闰年
 *
 * @param year
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int isleapyear(int year)
{
	if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	{
		return 1;
	}

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 将日期设置为本年的第几天
 *
 * @param pd
 * @param ydays
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateSetYDays(pdate *pd, int ydays)
{
	if(pd == NULL || ydays <= 0 || ydays > 365 + pd->isleap)
	{
		return -1;
	}

	int i = 1;
	for( i = 1; i < 13; i++)
	{
		if(arrydays[i] + (i > 2 && pd->isleap ? 1 : 0) >= pd->ydays)
		{
			pd->month = i;
			pd->day = pd->ydays - arrydays[i-1];

			if(pd->month > 2 && pd->isleap == 1)
			{
				pd->day -= 1;
			}

			if(pd->ydays != ydays)
			{
				pd->wday = (pd->wday + ydays - pd->ydays) % 7;
				if(pd->wday <= 0) pd->wday += 7;
				pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;
			}

			break;
		}
	}

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 将日期设置为当月的几号
 *
 * @param pd
 * @param day
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateSetDay(pdate *pd, int day)
{
	if(pd == NULL || day <= 0 || day > mdays[pd->month - 1])
	{
		return -1;
	}
	pd->ydays += day - pd->day;
	pd->wday = (pd->wday + day - pd->day) % 7;
	if(pd->wday <= 0) pd->wday += 7;

	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;
	pd->day = day;

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 增加天数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateAddDays(pdate *pd, int number)
{
	if(pd == NULL)
	{
		return -1;
	}

	if(number < 0)
	{
		return dateSubDays(pd, -1 * number);
	}
	else if(number == 0)
	{
		return 0;
	}

	int i = 0;
	if(number >= 365)
	{
		// 估计大概有多少年 
		int nyear = number / 365;
		// 返回天数的变化
		int rc = dateAddYears(pd, nyear);

		if(rc > number)
		{
			// 减去多加的天数
			dateSubDays(pd, rc - number);

		}
		else if(rc < number)
		{
			dateAddDays(pd, number - rc);
		}
	}
	else
	{
		if(365 + pd->isleap - pd->ydays >= number)
		{
			pd->ydays += number;
			for( i = pd->month ; i < 13; i++)
			{
				if(arrydays[i] + (i > 2 && pd->isleap ? 1 : 0) >= pd->ydays)
				{
					pd->month = i;
					pd->day = pd->ydays - arrydays[i-1];

					if(pd->month > 2 && pd->isleap == 1)
					{
						pd->day -= 1;
					}

					pd->wday = (pd->wday + number) % 7;
					if(pd->wday == 0) pd->wday = 7;

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

					break;
				}
			}

			return 0;
		}
		else
		{
			int dnum = number + pd->ydays - 365 - pd->isleap;
			pd->year += 1;
			pd->ydays = dnum;
			pd->isleap = isleapyear(pd->year);

			for( i = 1; i < 13; i++)
			{
				if(arrydays[i] + (i > 2 && pd->isleap ? 1 : 0) >= pd->ydays)
				{
					pd->month = i;
					pd->day = pd->ydays - arrydays[i-1];

					if(pd->month > 2 && pd->isleap == 1)
					{
						pd->day -= 1;
					}

					pd->wday = (pd->wday + number) % 7;
					if(pd->wday == 0) pd->wday = 7;

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

					break;
				}
			}

			return 0;
		}
	}

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 减少天数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateSubDays(pdate *pd, int number)
{
	if(pd == NULL)
	{
		return -1;
	}

	if(number < 0)
	{
		return dateAddDays(pd, -1 * number);
	}
	else if(number == 0)
	{
		return 0;
	}

	int i = 0;
	if(number >= 365)
	{
		// 估计大概有多少年 
		int nyear = number / 365;
		// 返回天数的变化
		int rc = dateSubYears(pd, nyear);

		if(rc > number)
		{
			dateAddDays(pd, rc - number);
		}
		else if(rc < number)
		{
			dateSubDays(pd, number - rc);
		}
	}
	else
	{
		if(pd->ydays > number)
		{
			pd->ydays -= number;
			for( i = pd->month ; i > 0; i--)
			{
				if(arrydays[i - 1] + (i > 3 && pd->isleap ? 1 : 0) < pd->ydays)
				{
					pd->month = i;
					pd->day = pd->ydays - arrydays[i-1];

					if(pd->month > 2 && pd->isleap == 1)
					{
						pd->day -= 1;
					}

					pd->wday = (pd->wday - number) % 7;
					if(pd->wday <= 0) pd->wday += 7;

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

					break;
				}
			}

			return 0;
		}
		else
		{
			pd->year -= 1;
			pd->isleap = isleapyear(pd->year);
			int dnum = 365 + pd->isleap - number + pd->ydays;
			pd->ydays = dnum;

			for( i = 12; i > 0; i--)
			{
				if(arrydays[i-1] + (i > 3 && pd->isleap ? 1 : 0) < pd->ydays)
				{
					pd->month = i;
					pd->day = pd->ydays - arrydays[i-1];

					if(pd->month > 2 && pd->isleap == 1)
					{
						pd->day -= 1;
					}

					pd->wday = (pd->wday - number) % 7;
					if(pd->wday == 0) pd->wday = 7;

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

					break;
				}
			}

			return 0;
		}
	}

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 增加周数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateAddWeeks(pdate *pd, int number)
{
	int totalday = number * 7;
	dateAddDays(pd, totalday);
	return totalday;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 减少周数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateSubWeeks(pdate *pd, int number)
{
	int totalday = number * 7;
	dateSubDays(pd, totalday);
	return totalday;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 增加月数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateAddMonths(pdate *pd, int number)
{
	if(pd == NULL)
	{
		return -1;
	}

	if(number < 0)
	{
		return dateSubMonths(pd, -1 * number);
	}
	else if(number == 0)
	{
		return 0;
	}

	int totalday = 0;
	int dyear = number / 12;
	int dmonth = number % 12;

	// save -- if the day of last date = oldday  set day

	while(dmonth--)
	{
		if(pd->month == 2 && pd->isleap == 1)
		{
			totalday += 29;
		}
		else
		{
			totalday += mdays[pd->month - 1];
		}

		if(pd->month == 12)
		{
			pd->year += 1;
			pd->month = 1;

			pd->isleap = isleapyear(pd->year);
		}
		else
		{
			pd->month += 1;
		}
	}

	//total = dateAddYears(pd, dyear);
	while(dyear--)
	{
		if(pd->month >= 3)
		{
			pd->year++;
			pd->isleap = isleapyear(pd->year);
		}
		else
		{
			pd->isleap = isleapyear(pd->year);
			pd->year++;
		}

		// before 3-1  if year is leap add 366 
		// alfter 3-1  if last year is leap add 366
		totalday += 365 + pd->isleap;
	}

	pd->isleap = isleapyear(pd->year);
	if(pd->month == 2 && pd->isleap)
	{
		if(pd->day > 29)
		{
			totalday -= pd->day - 29;
			pd->day = 29;
		}
	}
	else
	{
		if(pd->day > mdays[pd->month - 1])
		{
			totalday -= pd->day - mdays[pd->month - 1];
			pd->day = mdays[pd->month - 1];
		}
	}

	pd->wday = (pd->wday + totalday) % 7;
	if(pd->wday == 0) pd->wday = 7;

	pd->ydays = arrydays[pd->month - 1] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

	return totalday;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 减少月数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateSubMonths(pdate *pd, int number)
{
	if(pd == NULL)
	{
		return -1;
	}

	if(number < 0)
	{
		return dateAddMonths(pd, -1 * number);
	}
	else if(number == 0)
	{
		return 0;
	}

	int totalday = 0;
	int dyear = number / 12;
	int dmonth = number % 12;

	while(dmonth--)
	{
		if(pd->month == 1)
		{
			pd->year -= 1;
			pd->month = 12;

			pd->isleap = isleapyear(pd->year);
		}
		else
		{
			pd->month -= 1;
		}

		if(pd->month == 2 && pd->isleap == 1)
		{
			totalday += 29;
		}
		else
		{
			totalday += mdays[pd->month - 1];
		}

	}

	//total = dateAddYears(pd, dyear);
	while(dyear--)
	{
		if(pd->month < 3)
		{
			pd->year--;
			pd->isleap = isleapyear(pd->year);
		}
		else
		{
			pd->isleap = isleapyear(pd->year);
			pd->year--;
		}

		// before 3-1  if year is leap add 366 
		// alfter 3-1  if last year is leap add 366
		totalday += 365 + pd->isleap;
	}

	pd->isleap = isleapyear(pd->year);
	if(pd->month == 2 && pd->isleap)
	{
		if(pd->day > 29)
		{
			totalday += pd->day - 29;
			pd->day = 29;
		}
	}
	else
	{
		if(pd->day > mdays[pd->month - 1])
		{
			totalday += pd->day - mdays[pd->month - 1];
			pd->day = mdays[pd->month - 1];
		}
	}

	pd->wday = (pd->wday + totalday) % 7;
	if(pd->wday == 0) pd->wday = 7;

	pd->ydays = arrydays[pd->month - 1] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

	return totalday;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 增加年数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateAddYears(pdate *pd, int number)
{
	int dyear = number;
	int totalday = 0;
	while(dyear--)
	{
		if(pd->month >= 3)
		{
			pd->year++;
			pd->isleap = isleapyear(pd->year);
		}
		else
		{
			pd->isleap = isleapyear(pd->year);
			pd->year++;
		}

		totalday += 365 + pd->isleap;
	}

	pd->isleap = isleapyear(pd->year);
	if(pd->month == 2 && pd->isleap)
	{
		if(pd->day > 29)
		{
			totalday -= pd->day - 29;
			pd->day = 29;
		}
	}
	else
	{
		if(pd->day > mdays[pd->month - 1])
		{
			totalday -= pd->day - mdays[pd->month - 1];
			pd->day = mdays[pd->month - 1];
		}
	}

	pd->wday = (pd->wday + totalday) % 7;
	if(pd->wday == 0) pd->wday = 7;

	pd->ydays = arrydays[pd->month - 1] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

	return totalday;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 日期计算 减少年数
 *
 * @param pd
 * @param number
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateSubYears(pdate *pd, int number)
{
	int dyear = number;
	int totalday = 0;

	while(dyear--)
	{
		if(pd->month < 3)
		{
			pd->year--;
			pd->isleap = isleapyear(pd->year);
		}
		else
		{
			pd->isleap = isleapyear(pd->year);
			pd->year--;
		}

		// before 3-1  if year is leap add 366 
		// alfter 3-1  if last year is leap add 366
		totalday += 365 + pd->isleap;
	}

	pd->isleap = isleapyear(pd->year);
	if(pd->month == 2 && pd->isleap)
	{
		if(pd->day > 29)
		{
			totalday += pd->day - 29;
			pd->day = 29;
		}
	}
	else
	{
		if(pd->day > mdays[pd->month - 1])
		{
			totalday += pd->day - mdays[pd->month - 1];
			pd->day = mdays[pd->month - 1];
		}
	}

	pd->wday = (pd->wday - totalday) % 7;
	if(pd->wday <= 0) pd->wday += 7;

	pd->ydays = arrydays[pd->month - 1] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

	return totalday;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 计算两个日期之间的天数差
 *
 * @param pd1
 * @param pd2
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateDiff(pdate *pd1, pdate *pd2)
{
	int diff = 0;
	int y1 = pd1->year, y2 = pd2->year;
	int m1 = pd1->month, m2 = pd2->month;
	int d1 = pd1->day, d2 = pd2->day;
	int il1 = isleapyear(y1), il2 = isleapyear(y2);

	if(y1 < 0) y1 = 0;
	if(m1 <= 0) m1 = 1;
	if(m1 > 12) m1 = 12;
	if(d1 <= 0) d1 = 1;
	if(d1 > mdays[m1 - 1] + (m1 == 2 ? il1 : 0)) d1 = mdays[m1 - 1] + (m1 == 2 ? il1 : 0);

	if(y2 < 0) y2 = 0;
	if(m2 <= 0) m2 = 1;
	if(m2 > 12) m2 = 12;
	if(d2 <= 0) d2 = 1;
	if(d2 > mdays[m2 - 1] + (m2 == 2 ? il2 : 0)) d2 = mdays[m2 - 1] + (m2 == 2 ? il2 : 0);

	int yd1 = arrydays[m1 - 1] + d1, yd2 = arrydays[m2 - 1] + d2;
	
	if(m1 > 2) yd1 += il1;
	if(m2 > 2) yd2 += il2;

	if(y1 > y2)
	{
		diff += yd1;
		while( y2 <-- y1)
		{
			diff += 365 + isleapyear(y1);
		}

		diff += 365 - yd2 + il2;
	}
	else if(y1 < y2)
	{
		diff -= yd2;
		while(y1 <-- y2)
		{
			diff -= 365 + isleapyear(y2);
		}
		diff -= 365 - yd1 + il1;
	}
	else
	{
		diff = yd1 - yd2;
	}

	return diff;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 根据年月日创建一个pdate
 *
 * @param pd
 * @param year
 * @param month
 * @param day
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int dateInit(pdate *pd, int year, int month, int day)
{
	pd->year = year < 0 ? 0 : year;
	pd->month = month;
	pd->day = day;

	pd->isleap = isleapyear(pd->year);
	
	if(pd->month > 12) pd->month = 12;
	else if(pd->month <= 0) pd->month = 1;

	if(pd->day <= 0) pd->day = 1;
	else if(pd->day > mdays[pd->month - 1] + (pd->month == 2 ? pd->isleap : 0)) pd->day = mdays[pd->month - 1] + (pd->month == 2 ? pd->isleap : 0);

	pd->ydays = arrydays[pd->month - 1] + pd->day;
	if(pd->ydays > 2) pd->ydays += pd->isleap;

	pdate pda;
	pda.year = 2018;
	pda.month = 1;
	pda.day = 1;
	pda.weeks = 1;
	pda.wday = 1;
	pda.ydays = 1;
	pda.isleap = 0;
	int diff = dateDiff(pd, &pda);
	pd->wday = (diff + 1) % 7;
	if(pd->wday <= 0) pd->wday += 7;

	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7 + 1;

	return 0;
}

#define __PDATE_TEST_ 0
#if __PDATE_TEST_ 

int main()
{
	pdate pd;
	pd.year = 2018;
	pd.month = 1;
	pd.day = 1;
	pd.weeks = 1;
	pd.wday = 1;
	pd.ydays = 1;
	pd.isleap = 0;
	int rc;

	pdate pd2;
	rc = dateInit(&pd2, 2018, 1, 26);
	rc = dateSubDays(&pd2, 26);
	printf("%d-%d-%d(%d) rc = %d\n", pd2.year, pd2.month, pd2.day, pd2.wday, rc);

	rc = dateAddDays(&pd, 30);
	printf("%d-%d-%d(%d) rc = %d\n", pd.year, pd.month, pd.day, pd.wday, rc);
	
	rc = dateAddMonths(&pd, 13);
	printf("%d-%d-%d(%d) rc = %d\n", pd.year, pd.month, pd.day, pd.wday, rc);

	rc = dateAddYears(&pd, 2);
	printf("%d-%d-%d(%d) rc = %d\n", pd.year, pd.month, pd.day, pd.wday, rc);

	rc = dateSubYears(&pd, 14);
	printf("%d-%d-%d(%d) rc = %d\n", pd.year, pd.month, pd.day, pd.wday, rc);

	rc = dateAddYears(&pd, 14);
	printf("%d-%d-%d(%d) rc = %d\n", pd.year, pd.month, pd.day, pd.wday, rc);

	return 0;
}
#endif
