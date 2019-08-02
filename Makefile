CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-1 dbus-glib-1 gio-2.0 gio-unix-2.0 glib-2.0)
CFLAGS = -g -Wall -Werror

all: my_app_generated client_proxy_library client_proxy_share_library server client

my_app_generated: src/my_app_generated.c
	gcc -c -fPIC $(CFLAGS) -I include src/my_app_generated.c -o build/my_app_generated.o $(CFLAGS_DBUS_GLIB)

client_proxy_library: src/client_proxy_library.c
	gcc -c -fPIC $(CFLAGS) -I include src/client_proxy_library.c -o build/client_proxy_library.o $(CFLAGS_DBUS_GLIB)

client_proxy_share_library: build/client_proxy_library.o
	gcc -shared  build/client_proxy_library.o build/my_app_generated.o -o build/client_proxy_share_library.so $(CFLAGS_DBUS_GLIB)

server: src/my_app.c
	gcc $(CFLAGS) -I include src/my_app.c build/my_app_generated.o  -o build/my_app $(CFLAGS_DBUS_GLIB)

client: src/my_app_client.c
	gcc $(CFLAGS) -I include src/my_app_client.c  build/my_app_generated.o build/client_proxy_share_library.so -o build/my_app_clients $(CFLAGS_DBUS_GLIB)


clean:
	rm -f build/*
