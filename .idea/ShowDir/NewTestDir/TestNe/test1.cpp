int foo()
{
int *buf = "vcvc";
buf[sizeof(buf)] = "a";
buf[sizeof(buf)-1] = "a";

long a;
char x;
memcpy(buf,a,12);
memcpy(buf,x,12);
memcpy(x,buf,12);

//1.

char buf[512] = "this is a string";

buf += sizeof(buf);
buf += sizeof(buf)-1;

  

//2. 

char *cp1;

char buf[512] = "this is a string";

cp1 = buf;

cp1 += sizeof(buf);  

 

//3.

char *cp1, *cp2;

char buf[512] = "this is a string";

cp2 = cp1 = buf;

cp2 += sizeof(buf);  


chroot("/var/ftproot");
fgets(filename, sizeof(filename), network);
localfile = fopen(filename, "r");
while ((len = fread(buf, 1, sizeof(buf), localfile)) != EOF) {
  fwrite(buf, 1, sizeof(buf), network);
}
fclose(localfile);

buf = (char*) malloc(req_size);
    strncpy(buf, xfer, req_size);

	x = getBlock(a);

	char* rPort = getenv("rPort");
serv_addr.sin_port = htons(atoi(rPort));
if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
error("ERROR connecting");

	
}

	char* getBlock(int fd) {
  char* buf = (char*) malloc(BLOCK_SIZE);
  if (!buf) {
    return NULL;
  }
  if (read(fd, buf, BLOCK_SIZE) != BLOCK_SIZE) {
    return NULL;
  }
CString query = "SELECT * FROM items WHERE owner = '" 
				 + userName + "' AND itemname = '"  
				 + request.Lookup("item") + "'";
dbms.ExecuteSQL(query);
  
  return buf;
 
}

char *lccopy(const char *str) {
	char buf[BUFSIZE];
	char *p;

	strcpy(buf, str);
	for (p = buf; *p; p++) {
		if (isupper(*p)) {
			*p = tolower(*p);
		}
	}	
	x = EnterCriticalSection();
	y = _alloca();
	

	return strdup(buf);
	
}


#define BUFSIZE 256

int main(int argc, char **argv) {
  char buf[BUFSIZE];

  strcpy(buf, argv[1]);
}

#define BUFSIZE 256

int main(int argc, char **argv) {
  char *buf;

  buf = (char *)malloc(BUFSIZE);
  strcpy(buf, argv[1]);
}

#include <stdio.h>
#include <strings.h>
#include <wchar.h>

int main() {   
  wchar_t wideString[] = L"The spazzy orange tiger jumped " \ 
  "over the tawny jaguar.";    
  wchar_t *newString;    

  printf("Strlen() output: %d\nWcslen() output: %d\n", 
  strlen(wideString), wcslen(wideString));    

  // Very wrong for obvious reasons //    
  newString = (wchar_t *) malloc(strlen(wideString));    
  //*/    

  //Wrong because wide characters aren't 1 byte long! //    
  newString = (wchar_t *) malloc(wcslen(wideString));    
  //    

  /* correct! */    
  newString = (wchar_t *) malloc(wcslen(wideString) * sizeof(wchar_t));    

  /* ... */

char* ptr = (char*)malloc (SIZE);


if (err) {
    abrt = 1;
    free(ptr);
}
if (abrt) {
    logError("operation aborted before commit", ptr);
}  
 }
