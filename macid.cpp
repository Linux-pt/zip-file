
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<net/if.h>
#include <errno.h>
#include <cstring>

#ifdef MACOSX
#include<sys/sockio.h>
#include<net/if_dl.h>
#include <net/ethernet.h>
#endif

#include <string>
#include <vector>

#include "GetAllMACAddresses.h"
#include "GetMacAddress.h"


using namespace std;

#ifdef MACOSX

#define MAXADDRS 32
#define BUFFERSIZE 4000
#define max(a,b)((a) > (b) ? (a) : (b))

unsigned long           ip_addrsForNIC[MAXADDRS];
char                                    *hw_addrsForNIC[MAXADDRS];
char                                    *ip_namesForNIC[MAXADDRS];
char                                    *if_namesForNIC[MAXADDRS];

#else

#define PATH_MAX_LEN 256

#endif


int GetComputerName ( char *CName )
{

	char name[51];
	int ret;
	ret = gethostname ( name, 51 );
	for ( unsigned int i=0; i < strlen ( name ); i++ )
		if ( isalpha ( name[i] ) )
		{
			name[i]=toupper ( name[i] );
                }
  
    	strcpy ( CName, name );
        return ret;
}

/*********************************************************************************
 
 *********************************************************************************/
/*int GetOSDetails()
{
	FILE *fp;
	int status;
	char path[PATH_MAX_LEN];

	fp = popen("lsb_release", "r");
	if (fp == NULL)
	{	
		return -1;	
	}

	while (fgets(path, PATH_MAX_LEN, fp) != NULL)
		printf("%s", path);

	status = pclose(fp);
	if (status == -1) 
	{
	    // Error reported by pclose() 
	} 
	else 
	{
	    // Use macros described under wait() to inspect `status' in order
	    //        to determine success/failure of command executed by popen() 
	}

	return 0;
}	*/

/*********************************************************************************

**********************************************************************************/
/*int main()
{


	char Temp[51];	
	string Mac, CName;

        memset ( Temp, 0, sizeof ( Temp ) );
        if ( GetMacAddress ( Temp ) == 0 )
        {

    		Mac = Temp;
		printf("MAC ADDRESS = %s \n", Mac.c_str());	
        }
        else
        {
            printf ( "Error : Get Mac\n" );
        }


        memset ( Temp, 0, sizeof ( Temp ) );
        if ( GetComputerName ( Temp ) == 0 )
        {
            CName = Temp;
	    printf("Host Name = %s \n", CName.c_str());
        }
        else
        {
            printf ( " Error : Get HostName\n");
        }
	return 0;
}*/
