#include <stdlib.h>
#include  <string.h>

int main() {
	char InputArray1[1002];
	char InputArray2[1002];
	int FollowIndex[1002];
	FollowIndex[0] = -1;
	int lenInputArray1, lenInputArray2;
	int i, j;
	printf("input source string: ");
	gets(InputArray1);
	lenInputArray1 = strlen(InputArray1);
	printf("input pattern string: ");
	gets(InputArray2);
	lenInputArray2 = strlen(InputArray2);
	
	for (i = 1, j = -1; i < lenInputArray2;  i = i + 1) 
	{
		for (; j >= 0 && InputArray2[i] != InputArray2[j+1]; j = FollowIndex[j]);
		if (InputArray2[i] == InputArray2[j+1]) 
		{
            j = j + 1;
        }
		FollowIndex[i] = j;
	}
	int estimation = 0;
	for (i = 0, j = -1; i < lenInputArray1; i = i + 1) 
	{
		for (; j >= 0 && InputArray1[i] != InputArray2[j+1]; j = FollowIndex[j]);
		if (InputArray1[i] == InputArray2[j+1]) 
		{
            j = j + 1;
        }
		if (j + 1 == lenInputArray2) 
		{
			estimation = 1;
			printf("%d\n", i - j);
			j = FollowIndex[j];
		}
	}
	if (estimation == 0){
		printf("False\n");
    }
	return 0;
}
