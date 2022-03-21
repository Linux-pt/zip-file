
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

#define DEBUGZIP_FILE			"/var/MicroWorld/var/log/eScanDebugInfo.zip"
#define OSINFO_FILE				"/opt/MicroWorld/etc/debugosinfo.txt"
#define SOFTWAREINFO_FILE     "/opt/MicroWorld/etc/debugsoftwarelist.txt"
#define SYSTEMLOG_FILE     	"/var/log/system.log"
#define ESCANVERSION_FILE		"/opt/MicroWorld/etc/escan-ver"
#define ESCANLICENSE_FILE		"/opt/MicroWorld/etc/keys/microworld.keys"
#define ESCANCONFIG_FILE		"/opt/MicroWorld/etc/escan/escan.conf"
#define ESCANMONCONFIG_FILE	"/opt/MicroWorld/etc/escan/escanmon.conf"
#define ESCANMONLOG_FILE		"/var/MicroWorld/var/log/escanmon.log"
#define ESCANUPDATESLOG_FILE	"/var/MicroWorld/var/log/updates.log"
#define MWAGENTLOG_FILE		"/var/MicroWorld/var/log/mwagent.log"
#define KERNELLOG_FILE		"/var/log/kernel.log"
#define INSTALLOG_FILE		"/var/MicroWorld/var/log/Install.log"
#define AVSUPDATELOG_FILE	"/var/MicroWorld/var/log/update_avs.log"
#define AUTOUPDATELOG_FILE	"/var/MicroWorld/var/log/auto_updates.log"
#define AUTOUPDATEAVSLOG_FILE	"/var/MicroWorld/var/log/auto_update_avs.log"
#define UPDATESCONF_FILE	"/opt/MicroWorld/etc/updates.conf"
#define MWCDMON_FILE            "/var/MicroWorld/var/log/mwcdmon.log"
#define EPSDAEMON_FILE          "/var/MicroWorld/var/log/epsdaemon.log"
#define MWCPMON_FILE            "/var/MicroWorld/var/log/mwcpmon.log"
#define MWPBLOCKER_file         "/var/MicroWorld/var/log/mwblocker.log"
#define MWPRINTD_FILE           "/var/MicroWorld/var/log/printMonitor.log"
#define KMOD_FILE               "/var/MicroWorld/var/log/kmod.log"
#define KMODP_FILE              "/var/MicroWorld/var/log/kmodpb.log"
#define ESCANGUI_FILE           "/var/MicroWorld/var/log/escangui.log"
#define NFDAEMON_FILE           "/var/MicroWorld/var/log/nfdaemon.log"
#define UPDATENOW_FILE          "/var/MicroWorld/var/log/updatenow.log"
#define RTM_FILE                "/var/MicroWorld/var/log/rtm.log"
#define LIBMWSHARE_FILE         "/var/MicroWorld/var/log/libmwshare.log"



#define MWCDMON_CONF            "/opt/MicroWorld/etc/mwcdmon.conf"
#define MWCPMON_CONF            "/opt/MicroWorld/etc/mwcpmon.conf"
#define EPSDAEMON_CONF          "/opt/MicroWorld/etc/epsdaemon.conf"
#define MWPBLOCKER_CONF         "/opt/MicroWorld/etc/websiteblock.conf"
#define MWPRINTD_CONF           "/opt/MicroWorld/etc/mwprintd.conf"
#define USBWHITELIST            "/opt/MicroWorld/etc/usbwhitelist"
#define USBBLACKLIST            "/opt/MicroWorld/etc/usbblacklist"
#define WINCLIENT               "/opt/MicroWorld/etc/winclient.conf"
#define RTM_CONF                "/opt/MicroWorld/etc/rtm.conf"
#define MWAGENT                 "/opt/MicroWorld/etc/mwagent.conf"
#define COMMONSETTINGS_CONF     "/opt/MicroWorld/etc/commonsettings.conf"
#define MWAV                    "/opt/MicroWorld/etc/mwav.conf"
#define MWAGENT                 "/opt/MicroWorld/etc/mwagent.conf"

//#include "asset.h"
//#include "softwarelist.h"
#include "zip.h"

int createDebugZipFile();

/*
int main()
{
//      string SwHwInfo;

		//Compute assets information
  //    MW_HW_INFO(SwHwInfo);
//		getPackageStatus( );

	//	ofstream assetsFile;
	//	assetsFile.open (OSINFO_FILE);
	//	assetsFile << "Assets Information" << endl;
	//	assetsFile << SwHwInfo;
	//	assetsFile.close();
		createDebugZipFile();

		return 0;
}



int createDebugZipFile()
{
   string zipFile;
   HZIP hz;
   hz = CreateZip( DEBUGZIP_FILE, 0 ); 
   ZipAdd(hz, "debugosinfo.txt", OSINFO_FILE );
   ZipAdd(hz, "debugsoftwarelist.txt", SOFTWAREINFO_FILE );
   ZipAdd(hz, "system.log", SYSTEMLOG_FILE );
   ZipAdd(hz, "escan-ver", ESCANVERSION_FILE );
   ZipAdd(hz, "microworld.keys", ESCANLICENSE_FILE );
   ZipAdd(hz, "escan.conf", ESCANCONFIG_FILE );
   ZipAdd(hz, "escanmon.conf", ESCANMONCONFIG_FILE );
   ZipAdd(hz, "escanmon.log", ESCANMONLOG_FILE );
   ZipAdd(hz, "updates.log", ESCANUPDATESLOG_FILE );
   ZipAdd(hz, "mwagent.log", MWAGENTLOG_FILE );
   ZipAdd(hz, "kernel.log", KERNELLOG_FILE );
   ZipAdd(hz, "Install.log", INSTALLOG_FILE );
   ZipAdd(hz, "update_avs.log", AVSUPDATELOG_FILE );
   ZipAdd(hz, "auto_updates.log", AUTOUPDATELOG_FILE );
   ZipAdd(hz, "auto_update_avs.log", AUTOUPDATEAVSLOG_FILE );
   ZipAdd(hz, "updates.conf", UPDATESCONF_FILE );
   ZipAdd(hz, "mwcdmon.log", MWCDMON_FILE );
   ZipAdd(hz, "mwcpmon.log", MWCPMON_FILE );
   ZipAdd(hz, "epsdaemon.log", EPSDAEMON_FILE );
   ZipAdd(hz, "mwblocker.log", MWPBLOCKER_file );
   ZipAdd(hz, "escangui.log", ESCANGUI_FILE );
   ZipAdd(hz, "kmod.log", KMOD_FILE );
   ZipAdd(hz, "kmodpb.log", KMODP_FILE );
   ZipAdd(hz, "libmwshare.log", LIBMWSHARE_FILE );
   ZipAdd(hz, "nfdaemon.log", NFDAEMON_FILE );
   ZipAdd(hz, "printMonitor.log", MWPRINTD_FILE );
   ZipAdd(hz, "updatenow.log", UPDATENOW_FILE );
   ZipAdd(hz, "rtm.log", RTM_FILE );
   ZipAdd(hz, "mwcdmon.conf", MWCDMON_CONF );
   ZipAdd(hz, "mwcpmon.conf", MWCPMON_CONF );
   ZipAdd(hz, "epsdaemon.conf", EPSDAEMON_CONF );
   ZipAdd(hz, "websiteblock.conf",MWPBLOCKER_CONF  );
   ZipAdd(hz, "mwprintd.conf", MWPRINTD_CONF );
   ZipAdd(hz, "usbwhitelist", USBWHITELIST );
   ZipAdd(hz, "usbblacklist", USBBLACKLIST);
   ZipAdd(hz, "winclient.conf",WINCLIENT  );
   ZipAdd(hz, "rtm.conf", RTM_CONF );
   ZipAdd(hz, "mwagent.conf", MWAGENT );
   ZipAdd(hz, "commonsettings.conf",COMMONSETTINGS_CONF  );
   ZipAdd(hz, "mwav.conf", MWAV );
   ZipAdd(hz, "mwagen.conf", MWAGENT );

   CloseZip(hz);
   return 0;
}
*/
