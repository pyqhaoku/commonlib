#ifndef __PDATE_H_
#define __PDATE_H_

typedef struct pdate_s{
	int year;
	int month;
	int day;

	int weeks; 
	int wday;  // 1 - 7   7 is sunday

	// the day number of year eg. 1.1 is 1 
	int ydays;

	int isleap;
}pdate;

int isleapyear(int year);
int dateSetYDays(pdate *pd, int ydays);
int dateDiff(pdate *pd1, pdate *pd2);
int dateInit(pdate *pd, int year, int month, int day);

int dateAddDays(pdate *pd, int number);
int dateSubDays(pdate *pd, int number);
int dateAddMonths(pdate *pd, int number);
int dateSubMonths(pdate *pd, int number);
int dateAddYears(pdate *pd, int number);
int dateSubYears(pdate *pd, int number);

#endif
