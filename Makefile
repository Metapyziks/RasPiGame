SRCDIR = src
BINDIR = bin

HDR = \
	$(SRCDIR)/lcd.h

SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/lcd.c

TARGET = main

CFLAGS = -O3 -Wall -Wcast-align -Wcast-qual -Wimplicit \
	   -Wmissing-declarations -Wmissing-prototypes -Wnested-externs \
	   -Wpointer-arith -Wredundant-decls -Wshadow \
	   -Wstrict-prototypes

$(BINDIR)/$(TARGET): $(SRC) $(HDR) Makefile
	mkdir $(BINDIR)
	gcc $(SRC) $(CFLAGS) -o $(BINDIR)/$(TARGET)
