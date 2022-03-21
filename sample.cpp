
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "zip.h"

int createDebugZipFile();


int main()
{
		createDebugZipFile();

		return 0;
}


int createDebugZipFile()
{
	char location[1023]="/tmp";
  // string zipFile;
   HZIP hz;
   hz = CreateZip( "/tmp/a.zip", 0 ); 
   ZipAdd(hz, "omkar.txt",NULL );
   CloseZip(hz);
   return 0;
}

