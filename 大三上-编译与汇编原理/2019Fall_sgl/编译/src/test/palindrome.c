#include <stdlib.h>
#include <string.h>

int main() {
	printf("Please input Char Array Under 1000 characters\n");
	char StringGet[1002];
	gets(StringGet);
	int len, i;
	len = strlen(StringGet);
	if (len < 0 || len > 1000) 
	{
		printf("Input Error, only one more chance:\n");
		gets(StringGet);
	}
	else 
	{
		int IsPLD = -1;
		for (i = 0; i + i < len && IsPLD != 1; i = i + 1) 
		{
			if (StringGet[len - 1 - i] != StringGet[i]) 
			{
                printf("False\n");
                IsPLD = 1;
            }
		}
        if (IsPLD != 1) {
        	printf("True\n");
        }
    }
	return 0;
}