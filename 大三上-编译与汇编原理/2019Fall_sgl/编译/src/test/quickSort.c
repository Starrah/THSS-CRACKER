#include <stdio.h>
int a[1001];
int InputArray; 
	
void QuickSort(int left,int right)
{
	int i, j, temp, t; 
	if(left > right)
    {
	    return;
    }
	temp = a[left];
	i = left;
	j = right;
	while(i != j)
	{
		while(a[j] >= temp && i<j )
		{
				j = j - 1;
		}
		while(a[i] <= temp && i < j){
				i = i + 1;
		}
		if(i < j) 
		{
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	a[left] = a[i];
	a[i] = temp;
    int temp1 = i-1, temp2= i+1;
	QuickSort(left, temp1);
	QuickSort(temp2, right); 
    return;
} 
 
int main(){
    printf("Input the array length:");
	scanf("%d",&InputArray);
	printf("Input every array element:");
	int i,j,t; 
	for(i = 0; i < InputArray; i = i + 1)
	{
		scanf("%d",&a[i]);	
	}	
	int temp = InputArray - 1;
	QuickSort(0,temp);
	
	for(i = 0; i < InputArray; i = i + 1)
	{
		printf( "%d ",a[i]);
	}
	return 0;
}
 