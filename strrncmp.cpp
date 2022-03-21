#include <stdio.h>
#include <string.h>
#include "WriteLogForlibmwshare.h"

///Compares the string in reverse order.
/**
 * \param str1 String
 * \param str2 String
 */
char* strrncmp(char *str1, char *str2)
{
	int len1, len2;
	if(str1 == NULL || str2 == NULL)
	{
		WriteLogForlibmwshare(2, "Can not compare a null string");
		return NULL;
	}
	len1 = strlen(str1);
	len2 = strlen(str2);
	while(len1-- > 0 && len2-- > 0)
	{
		if(str1[len1] != str2[len2])
		{
			return NULL;
		}
	}
	return(str1 + (strlen(str1) - strlen(str2)));
}
