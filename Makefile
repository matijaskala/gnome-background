CFLAGS=$(shell pkg-config --cflags gail-3.0 gnome-desktop-3.0) -Wall
LDLIBS=$(shell pkg-config --libs gail-3.0 gnome-desktop-3.0)

background: desktop-background.c desktop-window.c main.c
	$(CC) $(CFLAGS) $(LDLIBS) -o background desktop-background.c desktop-window.c main.c
