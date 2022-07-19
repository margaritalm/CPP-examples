# include <iostream>
# include <stdio.h>
using namespace std;

void func(char *src)
{
  char dst[(strlen(src) + 1) * sizeof(char)];
  strncpy(dst, src, strlen(src) + sizeof(char));
  //strcpy(dst, src);
  s = dst;
  dst[strlen(dst)] = 0;
}
