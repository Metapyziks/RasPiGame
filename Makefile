SRCDIR = src
BINDIR = bin

HDR = \
	$(SRCDIR)/utils.h \
	$(SRCDIR)/lcd.h \
	$(SRCDIR)/sprite.h

SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/lcd.c \
	$(SRCDIR)/sprite.c

TARGET = main

CFLAGS = -std=c99 -O3 -Wall -Wcast-align -Wcast-qual -Wimplicit \
	   -Wmissing-declarations -Wmissing-prototypes -Wnested-externs \
	   -Wpointer-arith -Wredundant-decls -Wshadow \
	   -Wstrict-prototypes

$(BINDIR)/$(TARGET): $(SRC) $(HDR) Makefile
	mkdir -p $(BINDIR)
	gcc $(SRC) $(CFLAGS) -o $(BINDIR)/$(TARGET)
