#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* ----------------------------------------------------------------------------*/
/**
 * @brief 随机产生一个NUM长度的整数数组 数组范围为[low,high]
 *
 * @param num
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int *generateArray(int num, int low, int high)
{
	int *result = malloc( sizeof(int) * num);
	srand( time(NULL));
	int i = 0;
	for( i = 0; i < num; i++)
	{
		if(low == high && low == 0)
			result[i] = rand();
		else
			result[i] = low + rand() % (high - low + 1);
	}

	return result;
}

int testQSort(int array[], int num, int (*qsort)(int a[], int low, int high))
{
	int *array_copy = malloc( num * sizeof(int));
	memcpy(array_copy, array, num * sizeof(int));

	struct timeval tnow, tend;
	gettimeofday(&tnow, NULL);

	qsort(array_copy, 0, num - 1);

	gettimeofday(&tend, NULL);

	long int ds = (tend.tv_sec - tnow.tv_sec) * 1000000 + tend.tv_usec - tnow.tv_usec;
	printf("used %ld us\n", ds);

#ifdef DEBUG
	int i = 0;
	for( i = 0; i < num ; i++)
	{
		printf("%d ", array_copy[i]);
	}
	printf("\n");
#endif
	free(array_copy);
	
	return ds;
}

int insertSort(int array[], int low, int high)
{
	int i = low;
	for( i = low; i < high; i++)
	{
		int min = array[i], mini = i;
		int j = low + 1;
		for( j = i + 1; j <= high; j++)
		{
			if(min > array[j]) min = array[j], mini = j;
		}

		if(i != mini)
		{
			array[mini] = array[i];
			array[i] = min;
		}
	}

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 快速排序 从小到大
 *
 * @param array[]
 * @param low
 * @param high
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int quickSort1(int array[], int low, int high)
{
	int left = low, right = high;
	int prot = array[low];

	if(left >= right) return 0;
	if(left == (right - 1))
	{
		if(array[left] > array[right])
		{
			array[low] = array[right];
			array[right] = prot;
			return 0;
		}
		else return 0;
	}

	while(left < right)
	{
		while(left < right && array[right] >= prot) right--;
		array[left] = array[right];

		while(left < right && array[left] < prot) left++;
		array[right] = array[left];
	}

	array[left] = prot;

	quickSort1(array, low, left - 1);
	quickSort1(array, right + 1, high);

	return 0;
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief 三数取中
 *
 * @param array[]
 * @param low
 * @param high
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------*/
/**
 * @brief 三数取中修改 找到low high mid位置中的中值 并使得 mid < low < high 
 * 避免倒序时 出现的递归层数太多 因为low位置始终为最大值 层数=(high-low)/2
 *
 * @param array[]
 * @param low
 * @param high
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int quickSort2(int array[], int low, int high)
{
	int left = low, right = high;
	if(left >= right) return 0;
	if(left == (right - 1))
	{
		if(array[left] > array[right])
		{
			int temp = array[low];
			array[low] = array[right];
			array[right] = temp;
			return 0;
		}
		else return 0;
	}

#ifdef DEBUG
	int di = 0;
	printf("%d:%d [", low, high);
	for(di = low; di <= high; di++)
	{
		printf("%d,", array[di]);
	}
	printf("]\n");
#endif
	int mid = (left + right) / 2;

	if(array[left] > array[right])
	{
		// left > right
		int temp = array[left];
		array[left] = array[right];
		array[right] = temp;
	}

	if(array[mid] > array[right])
	{
		// right > mid
		int temp = array[mid];
		array[mid] = array[right];
		array[right] = temp;
	}

	if(array[left] < array[mid])
	{
		// mid < left
		int temp = array[left];
		array[left] = array[mid];
		array[mid] = temp;
	}

	int prot = array[low];

	while(left < right)
	{
		while(left < right && array[right] >= prot) right--;
		array[left] = array[right];

		while(left < right && array[left] < prot) left++;
		array[right] = array[left];
	}

	array[left] = prot;

	quickSort2(array, low, left - 1);
	quickSort2(array, right + 1, high);

	return 0;
}

int quickSort3(int array[], int low, int high)
{
	int left = low, right = high;
	if(left >= right - 10)
	{
		insertSort(array, left, right);
		return 0;
	}

	int mid = (left + right) / 2;

	if(array[left] > array[right])
	{
		// left > right
		int temp = array[left];
		array[left] = array[right];
		array[right] = temp;
	}

	if(array[mid] > array[right])
	{
		// right > mid
		int temp = array[mid];
		array[mid] = array[right];
		array[right] = temp;
	}

	if(array[left] < array[mid])
	{
		// mid < left
		int temp = array[left];
		array[left] = array[mid];
		array[mid] = temp;
	}

	int prot = array[low];

	while(left < right)
	{
		while(left < right && array[right] >= prot) right--;
		array[left] = array[right];

		while(left < right && array[left] < prot) left++;
		array[right] = array[left];
	}

	array[left] = prot;

	int i = low;
	for( i = high; i > right; i--)
	{
		if(array[i] == prot)
		{
			right++;
			array[i] = array[right];
			array[right] = prot;
		}
	}

	quickSort3(array, low, left - 1);
	quickSort3(array, right + 1, high);

	return 0;
}

int quickSort4(int array[], int low, int high)
{
	int left = low, right = high;
	if(left >= right) return 0;
	if(left >= right - 10)
	{
		insertSort(array, left, right);
		return 0;
	}

#ifdef DEBUG
	int di = 0;
	printf("%d:%d [", low, high);
	for(di = low; di <= high; di++)
	{
		printf("%d,", array[di]);
	}
	printf("]\n");
#endif
	int mid = (left + right) / 2;

	if(array[left] > array[right])
	{
		// left > right
		int temp = array[left];
		array[left] = array[right];
		array[right] = temp;
	}

	if(array[mid] > array[right])
	{
		// right > mid
		int temp = array[mid];
		array[mid] = array[right];
		array[right] = temp;
	}

	if(array[left] < array[mid])
	{
		// mid < left
		int temp = array[left];
		array[left] = array[mid];
		array[mid] = temp;
	}

	int prot = array[low];

	while(left < right)
	{
		while(left < right && array[right] >= prot) right--;
		array[left] = array[right];

		while(left < right && array[left] < prot) left++;
		array[right] = array[left];
	}

	array[left] = prot;

	quickSort4(array, low, left - 1);
	quickSort4(array, right + 1, high);

	return 0;
}

int swap(int array[], int i, int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
	return 0;
}

int dualPivotSort(int array[], int low, int high)
{
	int less = low, more = high, pvot, qvot;

	if(less >= more) return 0;
	if(less >= more - 10)
	{
		insertSort(array, less, more);
		return 0;
	}
	
	if(array[low] > array[high])
	{
		swap(array, low, high);
	}

	pvot = array[less];
	qvot = array[more];
	int i = 0;

	//while(array[less] > pvot) less++;
	//while(array[more] <= qvot) more--;

	/*
	 * partitioning
	 *
	 * +------------------------------------------+
	 * | < pvot | pvot <= && < qvot | ? | >= qvot |
	 * +------------------------------------------+
	 *          ^                   ^   ^
	 *          |                   |   |
	 *          less                i   more
	 */

	for( i = less + 1 ; i <= more; i++)
	{
		if(array[i] < pvot)
		{
			// move to left
			swap(array, i, less);
			less++;
		}
		else if(array[i] >= qvot)
		{
			// move to right

			while(more > less && array[more] >= qvot) more--;
			if(more <= i) break;

			if(array[more] < pvot)
			{
				swap(array, more, less);
				less++;

				// the less is equal i
				// beause of the less is the first number >= pvot and not >= qvot 
				if(array[more] > qvot) more--;
				else swap(array, more, i), more--;
			}
			else
			{
				// find a number >= pvot and < qvot
				// swap the i to here
				swap(array, more, i);
				more--;
			}
		}
	}

	dualPivotSort(array, low, less - 1);
	dualPivotSort(array, less, more);
	dualPivotSort(array, more + 1, high);
	return 0;
}

int compareInt(const void *a, const void *b)
{
	return *(int *) a - *(int *) b;
}

int testQSort2(int array[], int num)
{
	/// test stdlib qsort
	
	int *array_copy = malloc( num * sizeof(int));
	memcpy(array_copy, array, num * sizeof(int));

	struct timeval tnow, tend;
	gettimeofday(&tnow, NULL);

	qsort(array_copy, num, sizeof(int), compareInt);

	gettimeofday(&tend, NULL);

	long int ds = (tend.tv_sec - tnow.tv_sec) * 1000000 + tend.tv_usec - tnow.tv_usec;
	printf("qsort:used %ld us\n", ds);

#ifdef DEBUG
	int i = 0;
	for( i = 0; i < num ; i++)
	{
		printf("%d ", array_copy[i]);
	}
	printf("\n");
#endif
	free(array_copy);
	/// end
	return 0;
}

int main()
{
	int num = 1000 * 10000;
	int i;

	//num = 10000;
	int *array = generateArray(num, 0, 0);
	printf ("create Array\n");

	printf("quickSort1:");
	testQSort(array, num, quickSort1);

	printf("quickSort2:");
	testQSort(array, num, quickSort2);

	printf("quickSort3:");
	testQSort(array, num, quickSort3);

	printf("quickSort4:");
	testQSort(array, num, quickSort4);

	printf("dualPivotSort:");
	testQSort(array, num, dualPivotSort);

	testQSort2(array, num);

	free(array);

	array = generateArray(num, 1, 1);
	printf("create Array of 1\n");

	printf("quickSort3:");
	testQSort(array, num, quickSort3);

	testQSort2(array, num);

	free(array);

	array = generateArray(num, 1, 10000);
	printf("create Array of [1,10000]\n");

	printf("quickSort3:");
	testQSort(array, num, quickSort3);

	printf("dualPivotSort:");
	testQSort(array, num, dualPivotSort);

	testQSort2(array, num);

	//free(array);

	for( i = 0; i < num ; i++)
	{
		array[i] = i;
	}
	printf("\ncreate Array of array[i] = i\n");

	printf("quickSort3:");
	testQSort(array, num, quickSort3);

	printf("dualPivotSort:");
	testQSort(array, num, dualPivotSort);

	testQSort2(array, num);

	//free(array);

	i = 0;
	for( i = 0; i < num ; i++)
	{
		array[i] = num - i;
	}
	printf("\ncreate Array of array[i] = %d - i\n", num);

	printf("quickSort3:");
	testQSort(array, num, quickSort3);

	printf("dualPivotSort:");
	testQSort(array, num, dualPivotSort);

	testQSort2(array, num);

	free(array);

	return 0;
}
