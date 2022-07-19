main()
{
	char str1[] = "To be or not to be";
	char str2[6];
	char str2[3];
	strncpy(str2,str1,sizeof(str2)-1);
	strncpy(str3,str1,sizeof(str2));
	strncpy(str3,str1,sizeof(str3));
	str2[5]='\0';
	puts (str2);

	char *cmd = new char[25];
	cin >> cmd; // Overflow

	char buf0[12];
    cin >> buf0;	// Overflow
    cout << "echo: " << buf0 << endl;
	
	string str12;
    cin >> str12;  // Not overflow
    cout << "str 1: " << str12 << endl;
	
	char buf1[12];
    cin.width(12);
    cin >> buf1;   // Not overflow
    cout << "echo: " << buf1 << endl;


	buf[sizeof(buf)] = '\0'; //whups - off by one! 
	buf[sizeof(buf)-1] = '\0'; //not off by one! 
	
	int x = sizeof1(c);
	n = x[sizeof(double)-1]; // overflow
	n = x[sizeof(int)]; // off by one

	n1 = x1[i];
	n1 = x1[s(i)];
	
	x = sizeof(x1);
	x = sizeof(char);
	y = sizeof t;
	
	a = 5 + sizeof(char);
	int b = 6 + sizeof z;
}
