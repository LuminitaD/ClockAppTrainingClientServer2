CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-1 dbus-glib-1 gio-2.0 gio-unix-2.0 glib-2.0)
CFLAGS = -g -Wall -Werror

all: myAppGenerated clientProxyLibrary clientProxyShareLibrary server client

myAppGenerated: src/myApp_generated.c
	gcc -c -fPIC $(CFLAGS) -I include src/myApp_generated.c -o build/myApp_generated.o $(CFLAGS_DBUS_GLIB)

clientProxyLibrary: src/clientProxyLibrary.c
	gcc -c -fPIC $(CFLAGS) -I include src/clientProxyLibrary.c -o build/clientProxyLibrary.o $(CFLAGS_DBUS_GLIB) 

clientProxyShareLibrary: build/clientProxyLibrary.o
	gcc -shared  build/clientProxyLibrary.o build/myApp_generated.o -o build/clientProxyShareLibrary.so $(CFLAGS_DBUS_GLIB) 

server: src/myApp.c
	gcc $(CFLAGS) -I include src/myApp.c build/myApp_generated.o  -o build/myApp $(CFLAGS_DBUS_GLIB) 

client: src/myAppClient.c
	gcc $(CFLAGS) -I include src/myAppClient.c  build/myApp_generated.o build/clientProxyShareLibrary.so -o build/myAppClient $(CFLAGS_DBUS_GLIB) 


clean:
	rm -f build/*
