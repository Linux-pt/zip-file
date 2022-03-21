
#ifdef MACOSX

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#include "softwarelist.h"
#include "MW_strtrim.h"

#define TEMP_SOFTWARELIST_FILE "/opt/MicroWorld/etc/debugsoftwarelist.txt.tmp"
#define SOFTWARELIST_DATAFILE  "/var/MicroWorld/var/tmp/debugsoftwarelist_data.txt"

#define BUFFER_SIZE 500

static struct node *package_list = NULL;

/******************************************************************************

 Deletes all node(s) if any, pointed by 'package_list'

******************************************************************************/
void deletePackageList()
{
	struct node *tmp = NULL;

	while(package_list != NULL)				//Is pointing to a node?
	{
		tmp = package_list;						//Point to the first node
		package_list = package_list->next;	//Leave first node from linked list
		if(tmp->name)								
		{
			free(tmp->name);						//Free node's member(s)
		}
		free(tmp);									//Free node itself
	}
}

/******************************************************************************

	 Adds a new node at the beginning of linked list pointed by 'package_list'

******************************************************************************/
int addNode( const char *packageName, const char *installationDate, char status )
{	
	struct node *iterator = NULL, *tmp = NULL;

	iterator = package_list;					//Point to the first node
	while(iterator != NULL)						//Is pointing to a node?
	{
		if(strcmp(iterator->name, packageName) == 0 && iterator->status == status)
		{
			return 0;								//Return if same node already exists
		}
		iterator = iterator->next;				//Point to the next node
	}
	tmp = (struct node *) malloc(sizeof(struct node));		//Allocate space to the new node
	if(tmp == NULL)
	{
		return -1;
	}
	tmp->name = (char *) malloc(strlen(packageName) + 1);	//Allocate space to member(s)
	if(tmp->name == NULL)
	{
		return -1;
	}
	strcpy(tmp->name, packageName);			//Fill data into the node
	strcpy(tmp->date, installationDate);
	tmp->status = status;
	tmp->next = package_list;					//Link linked list with new node
	package_list = tmp;							//Point new node by header pointer
	return 0;
}

//******************************************************************************
// Lists all currentlly installed package name(s) in TEMP_SOFTWARELIST_FILE file
// Adds newlly installed package name(s) to the linked list
//******************************************************************************
int addInstalledPackageNames()
{
	//int isPackageFound = 0;
	char oldLine[BUFFER_SIZE], newLine[BUFFER_SIZE], installationDate[BUFFER_SIZE], 
	packageName[BUFFER_SIZE], lastModified[BUFFER_SIZE], *cPtr = NULL;
	int status = -1;
	struct stat st;

	sprintf(oldLine, "find /Applications -maxdepth 2 -iname *.app > %s 2> /dev/null", SOFTWARELIST_DATAFILE);
	status = system(oldLine);									//List all currentlly installed packages in a file
	status = WEXITSTATUS(status);
	if( status != 0 )
	{
		return -1;
	}	

	FILE *fp = fopen(SOFTWARELIST_DATAFILE, "r");		//Use file that has all currentlly installed package name(s)
	if(fp == NULL)
	{
		return -1;
	}
	/*FILE *oldList = fopen(SOFTWARELIST_FILE, "r");		//Use file that has all package names which were installed last time
	if(oldList == NULL)
	{
		oldList = fopen(SOFTWARELIST_FILE, "w+");			//Use an empty file if old file is does not exist
		if(oldList == NULL)
		{
			fclose(fp);
			return -1;
		}
	}*/
	FILE *newList = fopen(TEMP_SOFTWARELIST_FILE, "w+");	//Use a file to list package name(s) to reffer next time
	if(newList == NULL)
	{
		fclose(fp);
		//fclose(oldList);
		return -1;
	}

	while(fgets(newLine, BUFFER_SIZE, fp) != NULL)		//Read a currentlly installed package name
	{
		MW_strtrim(newLine);		
		cPtr = strstr(newLine, "\n");
		if(cPtr != NULL)
		{
			*cPtr = '\0';
		}
		if(stat(newLine, &st) == -1)							//Find out the installation date of this package
		{
			continue;
		}
		strftime(installationDate, BUFFER_SIZE, "[%d/%b/%Y] [%d/%m/%Y]", localtime(&(st.st_mtime)));
		strcpy(packageName, basename(newLine));			
		cPtr = strstr(packageName, ".app");
		if(cPtr != NULL)
		{
			*cPtr = '\0';
		}
		sprintf(oldLine, "%s\n", packageName);
		if(fputs(oldLine, newList) < 0)
		{
		}
		//isPackageFound = 0;
		//fseek(oldList, 0, SEEK_SET);

		/*
		while(fgets(oldLine, BUFFER_SIZE, oldList) != NULL)	//Read a package name which were installed last time
		{
			cPtr = strstr(oldLine, "\n");
			if(cPtr != NULL)
			{
				*cPtr = '\0';
			}
			if(strcmp(oldLine, packageName) == 0)					//Is name matches with last time installed package(s)
			{
				isPackageFound = 1;
				break;														//Stop comparing
			}
		}*/
		
		//Commented bby swapnil on 18 April 2015
		//if(isPackageFound == 0)	
		//{
			if(addNode(packageName, installationDate, 0) != 0)	//If not found, add to linked list as newlly installed package
			{
			}
		//}
	}

	if(fflush(newList) != 0)
	{
		fflush(newList);	//Trying again
	}
	//fclose(oldList);														//Stop using all file(s)
	fclose(newList);
	fclose(fp);
	if(unlink(SOFTWARELIST_DATAFILE) != 0)							//Remove file that contains currentlly installed package names
	{																			//Note: We have listed formated valid name(s) in another file
	}
	return 0;
}

//******************************************************************************
// Adds uninstalled package names(s) to the linked list
//******************************************************************************
int addUninstalledPackageNames()
{
	int isPackageFound = 0;
	FILE *oldFile = NULL, *newFile = NULL;
	char oldLine[BUFFER_SIZE], newLine[BUFFER_SIZE];

	oldFile = fopen(SOFTWARELIST_FILE, "r");						//Use file that has package(s) name which were installed last time
	if(oldFile == NULL)
	{
		oldFile = fopen(SOFTWARELIST_FILE, "w+");					//Use a empty file if old file does not exists
		if(oldFile == NULL)
		{
			return -1;
		}
	}
	newFile = fopen(TEMP_SOFTWARELIST_FILE, "r");				//Use file that has currentlly installed package name(s)
	if(newFile == NULL)
	{
		fclose(oldFile);
		return -1;
	}
	while(fgets(oldLine, BUFFER_SIZE, oldFile) != NULL)		//Read a package name that were installed last time
	{
		MW_strtrim(oldLine);
		isPackageFound = 0;
		fseek (newFile, 0, SEEK_SET);
		while(fgets(newLine, BUFFER_SIZE, newFile) != NULL)	//Read a package name which is currentlly installed
		{
			MW_strtrim(newLine);
			if(strcmp(oldLine, newLine) == 0)						//Is both the names matches?
			{
				isPackageFound = 1;		
				break;														//Stop matching
			}
		}
		if(isPackageFound == 0)
		{
			if(addNode(oldLine, "[00/000/0000] [00/00/0000]", 1) != 0)	//Add package name which does not found in currentlly installed list
			{
			}
		}
	}
	fclose(oldFile);														//Stop using all file(s)
	fclose(newFile);
	return 0;
}

//******************************************************************************
// Provides an interface to obtain header node of linked list
// Calls required functions in proper order
// Manages TEMP_SOFTWARELIST_FILE and SOFTWARELIST_FILE files
//******************************************************************************
struct node* getPackageStatus()
{
	if(addInstalledPackageNames() != 0)
	{
		deletePackageList();
		return NULL;
	}

	/*if(addUninstalledPackageNames() != 0)
	{
		deletePackageList();
		return NULL;
	}*/

	if(rename(TEMP_SOFTWARELIST_FILE, SOFTWARELIST_FILE) != 0)	//Overwrite currentlly installed package name(s) with old list
	{
		deletePackageList();
		return NULL;
	}

	return package_list;														//Return header node
}

#endif
