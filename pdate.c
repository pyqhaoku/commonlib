#include <stdio.h>

#include "pdate.h"

static int mdays[12]    =    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int arrydays[13] = {0, 31, 59, 90,120,151,181,212,243,273,304,334,365};

int isleapyear(int year)
{
	if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	{
		return 1;
	}

	return 0;
}

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
			}

			break;
		}
	}

	return 0;
}

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

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

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

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

					break;
				}
			}

			return 0;
		}
	}

	return 0;
}

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

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

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
				if(arrydays[i-1] + (i > 3 && pd->isleap ? 1 : 0) >= pd->ydays)
				{
					pd->month = i;
					pd->day = pd->ydays - arrydays[i-1];

					if(pd->month > 2 && pd->isleap == 1)
					{
						pd->day -= 1;
					}

					pd->wday = (pd->wday - number) % 7;
					if(pd->wday == 0) pd->wday = 7;

					pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

					break;
				}
			}

			return 0;
		}
	}

	return 0;
}

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

	int i = 0;

	int totalday = 0;
	int dyear = number / 12;
	int dmonth = number % 12;

	// save -- if the day of last date = oldday  set day
	int oldday = pd->day;

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

	pd->ydays = arrydays[pd->month] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

	return totalday;
}

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

	int i = 0;

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

	pd->ydays = arrydays[pd->month] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

	return totalday;
}

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

	pd->ydays = arrydays[pd->month] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

	return totalday;
}

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

	pd->ydays = arrydays[pd->month] + pd->day;
	pd->weeks = ((pd->ydays - pd->wday) + 6) / 7;

	return totalday;
}

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

	int rc = dateAddDays(&pd, 30);
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
