/*

A CGI tool allowing the use of form.find("input name") in scripts after running init()

example in main:

#include "cgi.cpp"

int main(int argc, char *argv[])
{
	cgi form = init();
	string cheese;

	form.find( "cheese", cheese );

	cout<<"<html>";
	cout<<cheese;
	cout<<"</html>";
}

would generate a page with the content of query or post input 'cheese' on it.

I don't know if my array use is secure or not, it supports a megabyte at the moment.

I'll begin optimization on return from vacation, any suggestions?

*/

//planned features:

//function 'text("filename")' outputs the file to a browser - can be used for blogs

//function 'cgi.record("name","filename")' puts the content of name at the end of file filename

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class cgi
{
	friend cgi init();

	private: string name; //post/query variable
	string content; //contents of post/query variable
	cgi *nextone; //nextone cgi. functions stop at NULL when searching for a certain name.

	public: cgi(string n=" ", string c=" ") //initialize the cgi
	{
		name = n;
		content = c;
		nextone = NULL;
	}

	void reset(string n, string c) //reset the cgi
	{
		name = n;
		content = c;
		nextone = NULL;
	}

	void add(string n, string c) //add a variable to the end
	{
		if (nextone==NULL)
		{
			nextone = new cgi(n,c);
		}
		else
			nextone->add(n,c);
	}

	bool find(string n, string &c) //find n, and if found, return true and copy n's content to c. if not, return false
	{
		if ( ! name.compare( n ) ) //if name is equal to n
		{
			c = content;
			return true;
		}
		else if (nextone==NULL) //if this is the last element of the cgi
			return false;
		else //pass the find to the nextone 
			return nextone->find(n,c); 	
	}

	private: void convert()
	{
		int length = content.length();
		int con; //content position
		int nco=0; //new content position
		string newcontent;
		
		for (con = 0; con<length; con++)
		{
			if (content[con]=='%') //URL encoded, leave normally encoded things alone, convert what is safe to use in HTML
			{
				switch (content[con+1])
				{
					case '2':
						switch (content[con+2])
						{
							case '0': newcontent[nco++]=' '; continue;
							case '4': newcontent[nco++]='$'; continue;
							case '6': newcontent[nco++]='&'; continue;
							case 'b':
							case 'B': newcontent[nco++]='+'; continue;
							case 'c':
							case 'C': newcontent[nco++]=','; continue;
							case 'f':
							case 'F': newcontent[nco++]='/'; continue;

							default: newcontent[nco++] = '%';
							newcontent[nco++] = content[con++];
							newcontent[nco++] = content[con++];
							continue;
						}
						con+=2;
						nco++;
						continue;

					case '3':
						switch (content[con+2])
						{
							case 'a':
							case 'A': newcontent[nco++]=':'; continue;
							case 'b':
							case 'B': newcontent[nco++]=';'; continue;
							case 'd':
							case 'D': newcontent[nco++]='='; continue;
							case 'f':
							case 'F': newcontent[nco++]='?'; continue;

							default: newcontent[nco++] = '%';
							newcontent[nco++] = content[con++];
							newcontent[nco++] = content[con++];
							continue;
						}
						con+=2;
						nco++;
						continue;

					case '4':
						if (content[con+2]=='0')
							newcontent[nco++]='@';
						else
						{
							newcontent[nco++] = '%';
							newcontent[nco++] = content[con++];
							newcontent[nco++] = content[con++];
							continue;
						}
						con+=2;
						nco++;
						continue;

					default: newcontent[nco++] = '%';
					newcontent[nco++] = content[con++];
					newcontent[nco++] = content[con++];
					continue;
				}
			}
			else //if already plaintext
			{
				newcontent[nco] = content[con];
				nco++;
			}
		}
		newcontent[nco]='\0';
		content = newcontent;

		if (nextone!=NULL)
			nextone->convert();

	}
};

cgi init()
{
	cgi raw; //before url unencoding
	cgi plain; //after url unencoding
	string query; //in case of query string
	int length; //content length, if existent
	int a; //integer variable
	int b; //integer variable
	string c; //string variable
	string d; //string variable
	char buffer[1024000]; //support a megabyte of form data

	cout<<"Content-type: text/html\n\n"; //give the browser a heads up

	if ( getenv( "QUERY_STRING" ) ) //if there is a query string
	{
		query = getenv( "QUERY_STRING" );
	}

	else if ( getenv( "CONTENT_LENGTH" ) )
	{
		length = atoi( getenv( "CONTENT_LENGTH") );
		cin.read( buffer, length );
		query = buffer;
	}

	else
	{
		cout<<"Content-type: text/html\n\n";
		cout<<"No form input.";
	}

	//initialize the raw cgi

		a = query.find_first_of('='); //find the end of the variable name
		c = query.substr( 0 , a ); //assign c the variable name
		b = query.find_first_of('&'); //find the end of the variable content
		if (query.find_first_of('&') == -1 ) //if & not found
			b = query.length();
		d = query.substr( a+1 , b ); //assign d the variable content
		query = query.substr( b+1, query.length() ); //shorten the query string accordingly
		raw.reset( c, d );

		cout<<c<<" = "<<d<<endl<<"<br>";

		while (query.length()>1)
		{
			a = query.find_first_of('='); //find the end of the variable name
			if (query.find_first_of('=') == -1) //there are no more variables
				break;
			c = query.substr( 0 , a ); //assign c the variable name
			b = query.find_first_of('&'); //find the end of the variable content
			if (query.find_first_of('&') == -1 ) //if & not found
				b = query.length();
			d = query.substr( a+1 , b ); //assign d the variable content
			query = query.substr( b+1, query.length() ); //shorten the query string accordingly
			raw.add( c, d ); //add to the cgi instead of resetting it

			cout<<c<<" = "<<d<<"<br>";
		}

	//herein lies the conversion of all raw cgi content's to plaintext (not a grammatical error)

	raw.convert();
	plain = raw;

	//herein lies the returning of the converted content goodness

	return plain;
}