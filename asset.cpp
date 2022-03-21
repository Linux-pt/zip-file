

#include <stdio.h>
#include <stdlib.h>

#ifdef MACOSX
	#include <IOKit/IOKitLib.h>
	#include <signal.h>
#else
	#include <mntent.h>
#endif

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>

#include <time.h>


#include "check_os_64bit.h"
#include "macid.h"
#include "GetMacAddress.h"
#include "ltrim.h"
#include "asset.h"
#include "MW_strtrim.h"
#include "getValueOfCommand.h"
#include "getOSDetails.h"
#include "getIPAddresses.h"
#include "get_cpu_mem_info.h"
#include "describeSize.h"
#include "getFileSysStat.h"
#include "getInstallationDate.h"
#include "getOsInstallationId.h"

#define BUFFER					256
#define BUFFER_SIZE			512
#define LSBRELEASE_FILE		"/etc/lsb-release"
#define OSRELEASE_FILE		"/etc/os-release"
#define REDHATRELEASE_FILE	"/etc/redhat-release"

void  MW_HW_INFO(string&);

#ifdef MACOSX
void	get_platform_uuid(char * buf, int bufSize);
#endif


/*****************************************************************
	Mac OS X specific function to get 
	CPU model, processor Name and memory details
******************************************************************/
#ifdef MACOSX
int GetHwInfoMacOSX( char *ModelID, char *ProcessorName, char *Memory )
{
    char buf[PATH_MAX];
    if(system("system_profiler SPHardwareDataType > /var/MicroWorld/var/tmp/mwsystem_profiler.tmp") == -1)
    {
        return -1;
    }

    FILE *fp = popen("cat /var/MicroWorld/var/tmp/mwsystem_profiler.tmp | grep \"Model Identifier\" | awk '{print $3};'", "r");
    if (fp != NULL)
    {

	    if(fgets(buf, PATH_MAX, fp) != NULL)
	    {
	        char * pch;
	        pch = strstr (buf,"\n");
	        strncpy (pch,"\0",1);
	        ltrim(buf);
	        strcpy(ModelID, buf);
	    }
   	 pclose(fp);
	  }

    fp = popen("cat /var/MicroWorld/var/tmp/mwsystem_profiler.tmp | grep \"Processor Name\" | awk '{$1=\"\"; $2=\"\"; sub(\" \", \" \"); print}'","r");
    if (fp != NULL)
    {
	    if(fgets(buf, PATH_MAX, fp) != NULL)
	    {
	        char * pch;
	        pch = strstr (buf,"\n");
	        strncpy (pch,"\0",1);
	        ltrim(buf);
	        strcpy(ProcessorName, buf);
	    }
	    pclose(fp);
	 }

    fp = popen("cat /var/MicroWorld/var/tmp/mwsystem_profiler.tmp | grep \"Memory:\" | awk '{$1=\"\"; sub(\" \", \" \"); print}'","r");

    if (fp != NULL)
    {
		strcpy(Memory, "");
	   if(fgets(buf, PATH_MAX, fp) != NULL)
   	{
        char * pch;
        pch = strstr (buf,"\n");
        strncpy (pch,"\0",1);
        ltrim(buf);
        strcpy(Memory, buf);
    	  pclose(fp);
	   }

    } 

	return 0;
}
#endif

/*****************************************************************
******************************************************************/
/*
void MW_HW_INFO( string &SwHwInfo )
{
	string OSName, OSVersion, OSDescription, kernelVersion, result;
	unsigned int RaminMB;
	char macAddress[50], compName[256], ip[PATH_MAX];
	char cpuVendorID[BUFFER], cpuModelName[BUFFER], RAMSize[256];
	char insdate[100], partition[PATH_MAX];

	memset (compName, 0, sizeof (compName));
	if (GetComputerName(compName) != 0)
	{
		strcpy(compName, " ");
	}

	memset ( ip, 0, sizeof ( ip ) );
	if(getIPAddresses(ip) != 0)
	{
		strcpy(ip, " ");
	}

	memset ( macAddress, 0, sizeof ( macAddress ) );
	if ( GetMacAddress ( macAddress ) != 0 )
	{
		strcpy(macAddress, " ");
	}

	if(getOSDetails( OSName, OSVersion, OSDescription, kernelVersion ) == 0)
	{
		OSName += (OSName.length() == 0 ? "": " ");
	}

	#ifndef MACOSX
   	OSName += "Linux ";
	#endif

   OSName += OSVersion;

   int isOS64Bits = check_os_64bit();
   if( isOS64Bits >= 0 )
   {
      OSName += isOS64Bits ? " 64-Bit": " 32-Bit";
   }

	memset ( partition, 0, sizeof ( partition ) );
	getFileSysStat(partition);
	if(strlen(partition) < 1)
	{
		strcpy(partition, " ");
	}

	memset ( insdate, 0, sizeof ( insdate ) );
	getInstallationDate(insdate);

	memset( RAMSize, 0, 256 );
	memset( cpuModelName, 0, BUFFER );
	memset( cpuVendorID, 0, BUFFER );
	
	#ifdef MACOSX
	
	GetHwInfoMacOSX( cpuVendorID, cpuModelName, RAMSize );

	if( strlen(cpuVendorID) == 0 )
		strcpy(cpuVendorID, "Unavailable");
   if( strlen(cpuModelName) == 0 )
      strcpy(cpuModelName, "Unavailable");
   if( strlen(RAMSize) == 0 )
      strcpy(RAMSize, "Unavailable");
	

	#else
	if(get_cpu_mem_info( "/proc/meminfo", "MemTotal", result )  == 0 )
	{
		RaminMB = atoi(result.c_str()) / 1024;
		sprintf( RAMSize, "%u", RaminMB );
	}
	else
	{
		sprintf( RAMSize, "%s", " " );	
	}
	
	if( get_cpu_mem_info("/proc/cpuinfo","model name", result ) == 0)
	{
		strncpy(cpuModelName, result.c_str(), strlen(result.c_str()));
		cpuModelName[strlen(result.c_str()) ] = '\0';
	}
	else
	{
		strcpy(cpuModelName, " ");
	}

	if( get_cpu_mem_info("/proc/cpuinfo","vendor_id", result ) == 0 )
	{
		strncpy( cpuVendorID, result.c_str(), strlen(result.c_str()));
		cpuVendorID[strlen(result.c_str()) ] = '\0';
	}
	else
	{
		strcpy( cpuVendorID, " " );
	}
	#endif

   string osInstallationId;
#ifdef MACOSX
   char buf[512] = "";
   get_platform_uuid(buf, sizeof(buf));
   osInstallationId = buf;
#else
   if( getOsInstallationId( osInstallationId ) != 0 )
   {
      osInstallationId = "";
   }
#endif

	SwHwInfo  = string("Computer Name: ") + string(compName);
	SwHwInfo += string("\n");
	SwHwInfo += string("IP Address: ") + string(ip);
	SwHwInfo += string("\n");
	SwHwInfo += string("OS Name: ") + string(OSName);
	SwHwInfo += string("\n");
	SwHwInfo += string("Kernel Version: ") + string(kernelVersion);
	SwHwInfo += string("\n");
	SwHwInfo += string("OS Version: ") + string(OSVersion);
	SwHwInfo += string("\n");
	SwHwInfo += string("OS Install Date: ") + string(insdate);
	SwHwInfo += string("\n");
	SwHwInfo += string("\n");
	SwHwInfo += string("Total RAM: ") + string(RAMSize);
	#ifndef MACOSX
		SwHwInfo += string("MB");
	#endif
	SwHwInfo += string("\n");
	SwHwInfo += string("Cpu Model Name: ") + string(cpuModelName);
	SwHwInfo += string("\n");
	SwHwInfo += string("Cpu Vendor ID: ") + string(cpuVendorID);
	SwHwInfo += string("\n");
	SwHwInfo += string("Available Partitions: ") + string(partition);
	SwHwInfo += string("\n");
	SwHwInfo += string("Mac Address: ") + string(macAddress);
	SwHwInfo += string("\n");
	SwHwInfo += string("Os Installation Id: ") + osInstallationId;
	return;
}
*/

/*****************************************************************

	Returns unique Linux OS installation ID
	This is called as veryfyingnumber in windows

******************************************************************/
#ifdef MACOSX
void get_platform_uuid(char * buf, int bufSize)
{
   io_registry_entry_t ioRegistryRoot = IORegistryEntryFromPath(kIOMasterPortDefault, "IOService:/");
   CFStringRef uuidCf = (CFStringRef) IORegistryEntryCreateCFProperty(ioRegistryRoot, CFSTR(kIOPlatformUUIDKey), kCFAllocatorDefault, 0);
   IOObjectRelease(ioRegistryRoot);
   CFStringGetCString(uuidCf, buf, bufSize, kCFStringEncodingMacRoman);
   CFRelease(uuidCf);
}
#endif

