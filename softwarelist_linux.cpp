#ifndef MACOSX

#include <stdio.h>
#include <dirent.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include "MW_strtrim.h"

//#include "softwarelist_linux.h"
#include "softwarelist.h"
#include "strrncmp.h"

#define BUFFER_SIZE      1024
#define APPLICATIONS_DIR "/usr/share/applications"
#define SORTED_FILE		 "/opt/MicroWorld/etc/softwares.list.tmp"
#define UNSORTED_FILE	 "/opt/MicroWorld/etc/softwares.list.tmp_1"

static struct node *softwareList = NULL;

//************************************************************************************
/*int main()
{
	//char str1[BUFFER_SIZE] = "abc.desktop", str2[BUFFER_SIZE] = ".desktop", *cPtr = NULL;
	//cPtr = strrncmp(str1,str2);
	//if(cPtr != NULL)
	//{
	//	printf("%s\n", cPtr);
	//}

	if(getPackageStatus() == NULL)
	{
		printf("Empty!!!\n");
		return -1;
	}
	struct node *iter = NULL;
	iter = softwareList;	
	while( iter != NULL)
	{
		struct node *temp;
		temp = iter;
		printf("%s %s %d\n", iter->name, iter->date, (int)iter->status);
		iter = iter->next;
		free(temp->name);
		free(temp);
	}
	return 0;
}*/

//************************************************************************************

//This function provides interface to get notification list.
//Gives a valid pointer to notification list on success, NULL otherwise.

//************************************************************************************

struct node* getPackageStatus()
{
	//char buffer[BUFFER_SIZE];

	deletePackageList();
	if(findInstalledSoftwares() != 0)
	{
		deletePackageList();
		return NULL;
	}
	if(findUninstalledSoftwares() != 0)
	{
		deletePackageList();
		return NULL;
	}
	if(rename(SORTED_FILE, SOFTWARELIST_FILE) != 0)
	{
		deletePackageList();
		return NULL;
	}
	return softwareList;
}

//************************************************************************************

//This function frees all nodes from nofification list.

//************************************************************************************

void deletePackageList()
{
	struct node *temp;
	
	while(softwareList != NULL)
	{
		temp = softwareList;
		softwareList = softwareList->next;
		free(temp->name);
		free(temp);
	}
	return;
}

//************************************************************************************

//This function adds all un-notified software information to notification list.
//It also list all currentlly installed software names in a file.

//************************************************************************************

int findInstalledSoftwares()
{
	DIR *directory;
	struct dirent *ent;
	struct stat stats;
	struct tm *timeinfo;
	char buffer[BUFFER_SIZE], filePath[BUFFER_SIZE], date[30], *cPtr = NULL;
	FILE *inputFile, *outputFile;

	outputFile = fopen(UNSORTED_FILE, "w+");
	if(outputFile == NULL)
	{
		return -1;
	}
	directory = opendir(APPLICATIONS_DIR);
	if(directory == NULL)
	{
		fclose(outputFile);
		return -1;
	}
	while ((ent = readdir (directory)) != NULL)
	{
		cPtr = strrncmp(ent->d_name, ".desktop");
		if(cPtr == NULL)
		{
			continue;
		}
		sprintf(filePath, "%s/%s", APPLICATIONS_DIR, ent->d_name);
		if(stat(filePath, &stats) != 0)
		{
			continue;
		}
		inputFile = fopen(filePath, "r");
		if(inputFile == NULL)
		{
			continue;
		}
		while(fgets(buffer, BUFFER_SIZE, inputFile) != NULL)
		{
			cPtr = strstr(buffer, "\n");
			if(cPtr != NULL)
			{
				*cPtr = '\0';
			}
			MW_strtrim(buffer);
			if(strncmp(buffer, "Name=", 5) != 0)
			{
				continue;
			}
			cPtr = strstr(buffer, "=");
			if(cPtr == NULL)
			{
				continue;
			}
			cPtr++;
			if(fputs(cPtr, outputFile) < 0)
			{}
			if(fputs("\n", outputFile) < 0)
			{}
			if(isNotifiedBefore(cPtr) == 1)
			{
				break;
			}
			timeinfo = localtime(&(stats.st_ctime));
			strftime(date, sizeof(date), "[%d/%b/%Y] [%d/%m/%Y]", timeinfo);
			if(addToNotificationList(cPtr, 0, date) != 0)
			{
				fclose(inputFile);
				closedir(directory);
				return -1;
			}
			break;
		}
		fclose(inputFile);
	}
	closedir(directory);
	fclose(outputFile);
	if(access(UNSORTED_FILE, F_OK) == 0)
	{
		sprintf(buffer, "sort -u -o %s %s && rm -f %s", SORTED_FILE, UNSORTED_FILE, UNSORTED_FILE);
		if(system(buffer) != 0)
		{}
	}
	return 0;
}

//************************************************************************************

//This function check that whether given software is notified before or not.
//It returns 1 for notified software, 0 otherwise

//************************************************************************************

int isNotifiedBefore(char *software)
{
	FILE *historyFile;
	char buffer[BUFFER_SIZE], *cPtr;

	historyFile = fopen(SOFTWARELIST_FILE, "r");
	if(historyFile == NULL)
	{
		//It might happened for first time, let's create new one 
		historyFile = fopen(SOFTWARELIST_FILE, "w+");
		if(historyFile == NULL)
		{
			return -1;
		}
	}
	while(fgets(buffer, BUFFER_SIZE, historyFile) != NULL)
	{
		cPtr = strstr(buffer, "\n");
		if(cPtr != NULL)
		{
			*cPtr = '\0';
		}
		if(strcmp(buffer, software) == 0)
		{
			fclose(historyFile);
			return 1;
		}
	}
	fclose(historyFile);
	return 0;
}

//************************************************************************************

//This function adds given information in notification list.

//************************************************************************************

int addToNotificationList(char *software, char status, char *date)
{
	if(software == NULL)
	{
		return -1;
	}
	struct node *temp = (struct node *) malloc(sizeof(struct node));
	if(temp == NULL)
	{
		return -1;
	}
	temp->name = (char *) malloc( sizeof(char) * ( strlen(software) + 1 ) );
	if(temp->name == NULL)
	{
		free(temp);
		return -1;
	}
	strcpy(temp->name, software);
	strcpy(temp->date, date);
	temp->status = status;
	temp->next = softwareList;
	softwareList = temp;
	return 0;
}

//************************************************************************************

//This function adds all un-installed software information to notification list.

//************************************************************************************

int findUninstalledSoftwares()
{
	FILE *historyFile, *outputFile;
	int isEntryFound;
	char buffer[BUFFER_SIZE], oldPackageName[BUFFER_SIZE], newPackageName[BUFFER_SIZE], *cPtr = NULL;

	historyFile = fopen(SOFTWARELIST_FILE, "r");
	if(historyFile == NULL)
	{
		return -1;
	}
	outputFile = fopen(SORTED_FILE, "r");
	if(outputFile == NULL)
	{
		fclose(historyFile);
		return -1;
	}

	while(fgets(oldPackageName, BUFFER_SIZE, historyFile) != NULL)
	{
		isEntryFound = 0;
		if(fseek(outputFile, 0, SEEK_SET) != 0)
		{
			fclose(outputFile);
			fclose(historyFile);
			return -1;
		}
		cPtr = strstr(oldPackageName, "\n");
		if(cPtr != NULL)
		{
			*cPtr = '\0';
		}
		while(fgets(newPackageName, BUFFER_SIZE, outputFile) != NULL)
		{
			cPtr = strstr(newPackageName, "\n");
			if(cPtr != NULL)
			{
				*cPtr = '\0';
			}
			if(strcmp(oldPackageName, newPackageName) == 0)
			{
				isEntryFound = 1;
				break;
			}
		}
		if(isEntryFound == 0)
		{
			strcpy(buffer, "[00/000/0000] [00/00/0000]");
			if(addToNotificationList(oldPackageName, 1, buffer) != 0)
			{
				//fclose(historyFile);
				//return -1;
			}
		}
	}
	fclose(historyFile);
	fclose(outputFile);
	return 0;
}

/*
// ************************************************************************************
char* MW_strltrim(char *str)
{
	unsigned int p = 0;
	if(str != NULL)
	{
		while(isspace(str[p++]) && p < strlen(str));
		if(str[p] == '\0' && isspace(str[p - 1]))
			str[0] = 0;
		else if(--p > 0)
			strncpy(str, str + p, strlen(str));
	}

	return str;
}

// ************************************************************************************
char* MW_strrtrim(char *str)
{
	int p;
	if(str != NULL)
	{
		p = strlen(str) - 1;
		while(isspace(str[p]) && --p >= 0);
		str[++p] = 0;
	}
	return str;
}

// ************************************************************************************
char* MW_strtrim(char *str)
{
	if(str != NULL)
	{
		str = MW_strltrim(str);
		str = MW_strrtrim(str);
	}

	return str;
}
*/


#endif
