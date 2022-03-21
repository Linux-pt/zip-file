#!/bin/sh

#Use below flag for Mac OS X only, otherwise comment below line.
MAC_FLAG=-DZIP_STD
#MAC_FLAG=

CC=g++  $(MAC_FLAG) 
GCC=g++ $(MAC_FLAG) 
LIB_SFL_PATH=../sfl/
MWLIBPATH=/opt/MicroWorld/lib/

#LIBS=/opt/MicroWorld/lib/libmwshare.so.1.0.0 /opt/MicroWorld/lib/libmwact.so.1.0.0 /opt/MicroWorld/lib/libmwsusp.so.1.0.0 /opt/MicroWorld/lib/libmwdmidecode.so  /opt/MicroWorld/lib/libescan.so.3.0.0 /opt/MicroWorld/lib/libssl.so.0.9.8 /opt/MicroWorld/lib/libcurl.so.4 -lcrypto /opt/MicroWorld/lib/libcrypto.so.4  /opt/Microworld/lib/libzip.so 


IFLAGS=-I${LIB_SFL_PATH}  -I../libmwshare/header/ -I../sfl -I../libescan/header -I../scan_module/include 

#LFLAGS= -Wl,-rpath=/opt/MicroWorld/lib/ ${LIB_SFL_PATH}/sfl.a -lm -ldl -lpthread ${MWLIBPATH}/libescan.so.3.0.0 -lcurl ${MWLIBPATH}/libmwshare.so.1.0.0 ${MWLIBPATH}/libmwact.so.1.0.0 ${MWLIBPATH}/libmwsusp.so.1.0.0 ${MWLIBPATH}/libmwdmidecode.so -lssl -lcrypto -lcurl -lgcrypt -lxml2



LFLAGS=../sfl/sfl.a /opt/MicroWorld/lib/libmwshare.so.1.0.0 /opt/MicroWorld/lib/libmwact.so.1.0.0 /opt/MicroWorld/lib/libmwsusp.so.1.0.0 /opt/MicroWorld/lib/libmwdmidecode.so  /opt/MicroWorld/lib/libescan.so.3.0.0 /opt/MicroWorld/lib/libssl.so.0.9.8 /opt/MicroWorld/lib/libcurl.so.4 -lpthread -lxml2 -lcrypto #/opt/MicroWorld/lib/libcrypto.so.4  /opt/MicroWorld/lib/libcrypto.so.0.9.8   


#IFLAGS= -I./headders/ -DXTHREADS -D_REENTRANT -DXUSE_MTSAFE_API -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include/directfb -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gtk-3.0 -I/usr/include/graphene-1.0 -I/usr/lib/x86_64-linux-gnu/graphene-1.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -I/usr/include/gdk-pixbuf-2.0  -I/usr/include/cairo -I/usr/lib64/gtk-4.0/include -I/usr/X11R6/include -I/usr/include/atk-1.0 -I/usr/include/pango-1.0 -I/usr/include/freetype2 -I/usr/include/freetype2/config -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -DQT_SHARED -I/usr/local/Trolltech/Qt-4.4.0/include -I/usr/local/Trolltech/Qt-4.4.0/include/QtCore -I/usr/local/Trolltech/Qt-4.4.0/include/QtGui -I/usr/include/qt4/QtCore/ -I/usr/include/qt4/ -I/usr/include/qt5/QtWidgets -I/usr/include/qt4/QtGui/  -shared -fPIC




 
#LFLAGS=${LIB_SFL_PATH}/sfl.a -lpthread

OUTPUT=hardwareinfo
OBJFILES= asset.o macid.o softwarelist_linux.o assetsmain.o zip.o sample.o

SOURCES=${OBJFILES:.o=.cpp}

.SUFFIXES: .cpp .o

.cpp.o:
	@echo "Compiling $*.cpp"
	@${CC} -DZIP_STD -c $< ${IFLAGS} -o $@   

all: $(OBJFILES)
	@echo "Generating $(OUTPUT)"
	@$(GCC) -DZIP_STD   $(OBJFILES)  -Wl,-rpath=/opt/Microworld/lib/ ${LFLAGS} -o  $(OUTPUT) #$(OBJFILES) 
	@strip $(OUTPUT)
	@echo Done

clean:
	@echo "Cleaning..."
	@rm -vf $(OUTPUT) $(OBJFILES)
	@echo "Cleaned!"
