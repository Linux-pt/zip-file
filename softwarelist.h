
#ifndef __SOFTWARELIST_H__
#define __SOFTWARELIST_H__

#define SOFTWARELIST_FILE			"/opt/MicroWorld/etc/debugsoftwarelist.txt"
#define LARGE_BUFFER					3000

struct node 
{
	char *name;
	char status;
	char date[30]; 
	struct node *next;
};

struct node*	getPackageStatus();
void				deletePackageList();
int				addToNotificationList(char *software, char status, char *date);
int				isNotifiedBefore(char *software);
int				findUninstalledSoftwares();
int				findInstalledSoftwares();
int				addInstalledPackageNames();
#endif //__SOFTWARELIST_H__
