void nul0()
{
 char *pProcessed;
 string uFileSize;
 string null1 = "";
 ufile = "";
 x = null;
 y = NULL;
 x = new char[];
 int i;
 int j = null; 
}
	
void nul1()
{
	char *cp1, *cp2;
	char buf[512] = "this is a string";
	unsigned int i = sizeof(buf);

	cp1 = &buf[511];
	cp2 = buf;

	while(i  > -1)
	 *cp1-- = *cp2++;   // something’s going to go “BANG!” sometime.
}
//                It also lacks any pointer math checks that I can see, e.g:

void nul2()
{
	char *cp1, *cp2;
	char buf[512] = "this is a string";

	cp2 = cp1 = buf;
	cp2 += sizeof(buf);  
}

	private: String message;
	string message1;
