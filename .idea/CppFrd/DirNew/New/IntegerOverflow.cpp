void foo0()
{
 char *pProcessed;
 short uFileSize;
 pProcessed = new long[uFileSize]; // short overflow       *** not found ***
}

void foo1()
{
 long *pProcessed;
 short uFileSize;
 uFileSize = pProcessed[2]; // short overflow       *** not found ***
 }

void foo()
{
 char *pProcessed;
 long uFileSize;
 bool p1;
 bool *pp;
 pProcessed = new char[uFileSize]; // NO overflow
 p1 = pProcessed; // bool overflow
 p1 = pp[p1]; // NO overflow
 p1 = pProcessed[p1]; // bool overflow       *** not found ***
 p1 = pp[uFileSize]; // NO overflow
 p1 = pProcessed[uFileSize]; // bool overflow       *** not found ***
}

int foo()
{


	int x = a;
	double y;

	x = y; // This overflow is detected

	int z = y; // This overflow is not detected, because it's in an initializer

	
//	void foo(int a=y) { }  // does this work?

	int nPopSize;
	cin>>nPopSize;
for( int nPopIndex = 0; nPopIndex < nPopSize; nPopIndex++ )
{
                     szNewFile = new char[(strlen(OFFSPRING_PATH) + strlen(OFFSPRING_FILE) + strlen("0000.xml") + 1)];
                     strcpy(szNewFile, OFFSPRING_PATH);
                     strcat(szNewFile, OFFSPRING_FILE);
                     strcat(szNewFile,itoa(nPopIndex, szNumber,10));
                     strcat(szNewFile,".xml");
}


	bool b1,b2,b3;
	short s1, s2, s3;
	char c1, c2, c3;
	int i1,i2,i3;
	float f1, f2, f3;
	long l1, l2, l3;
	double d1, d2, d3;
	
	b2 = b2 + (f1>l2);
	b3 = b1+b2 - 1;
	b1 = b2+d3;
	b1 = b2+(d3=2);
	
	s1 = b1 + s2;
	s2 = s3*1000;
	s3 = c1+c2-(b3=2);
	s1 = (b1=s2)-2;
	s1 = (b1=f2)-2;
	s2 = (l1<l2) + (f1==1);
	s2 = (l1<l2) + (f1==1) / (l1=2);
	
	c1 = b1 + s2;
	c2 = l3*1000;
	c3 = c1+s2-(b3=2);
	c1 = (b1=s2)-2;
	c1 = (b1=f2)-2;
	c2 = (l1<l2) + (f1==1);
	c2 = (l1<l2) + (f1==1) / (l1=2);
	
	i1 = b1 + s2;
	i2 = c3*1000;
	i3 = c1+s2-(b3=2) + f2;
	i1 = (b1=s2)-2 * (f2!=3)?f1:f3;
	i1 = (b1=f2)-2;
	i2 = (l1<l2) + (f1==1);
	i2 = (l1<l2) + (f1==1) / (l1=2);
	
	f1 = b1 + s2;
	f2 = c3*1000;
	f3 = c1+d2-(b3=2) + f2;
	f1 = (b1=s2)-2 * (f2!=3)?f1:f3;
	f1 = (i1=d2)-2;
	f2 = (l1<l2) + (d1==1);
	f2 = (l1<d2) + (f1==1) / (l1=2);
	
	l1 = b1 + s2;
	l2 = c3*1000 + l1 + i2;
	l3 = c1+d2-(b3=2) + f2;
	l1 = (b1=s2)-2 * (f2!=3)?i1:b3;
	l1 = (i1=d2)-2;
	l2 = (l1<l2) + (d1==1);
	l2 = (l1<d2) + (f1==1) / (l1=2);

	return 1;
}