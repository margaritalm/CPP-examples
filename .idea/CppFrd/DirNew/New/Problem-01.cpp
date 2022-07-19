/*
The block code has the Format String issue,but Cxsuit can not find.
*/


#include <string.h>
#include <stdio.h>

#define MAX_SIZE 64

int main(char** argv, int argc)
{           
            char in[MAX_SIZE];
            int chars;
			SqlConnection myConn = new SqlConnection ( );
			myConn.ConnectionString = "user id=sa; password=yervze?un;" +
			   "initial catalog=northwind; data source=myServer";
			myConn.Open ( );            
            memset(in, 0, MAX_SIZE);
            //chars = read(0, in, MAX_SIZE-1);
            scanf("%70s", &in);
            printf("You just entered:");
            printf(in);
			
			
            return 0;
}

		int VerifyAdmin(char *password)
		{ 
			if (strcmp(password, "Mew!"))
			{ 
			  printf("Incorrect Password!\n"); 
			  return(0);
			}
			printf("Entering Diagnostic Mode…\n");
			return(1); 
		}

