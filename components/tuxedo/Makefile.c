# Makefile for Tuxedo /Q C Implementation

CC = gcc
TUXDIR = /u01/oracle/tuxHome/tuxedo22.1.1.0.0
CFLAGS = -I$(TUXDIR)/include -Wall -O2
LDFLAGS = -L$(TUXDIR)/lib -ltux -lbuft -lfml -lfml32 -lengine -lmicrohttpd -lpthread

TARGET = tuxedo_q_server
SRC = src/tuxedo_q_server.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
	@echo "Clean complete"

install: $(TARGET)
	install -m 755 $(TARGET) /u01/oracle/user_projects/tuxdemo/
	@echo "Installed to /u01/oracle/user_projects/tuxdemo/"
