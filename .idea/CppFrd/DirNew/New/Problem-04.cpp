/*
The block code has the buffer overflow, but Cxsuit can not find.
*/


#include <string.h>
#include <stdio.h>

#define MAX_SIZE 64

int main(char** argv, int argc)
{           
            char in[MAX_SIZE];
            char out[4096];
            int chars;
			short size;
			scanf("%d",*size);
			scanf("%d",size);
            memset(in, 0, size);
			
			float value;
			scanf("%d%f", size, value);
			memset(in, value, size);

            return 0;
}

