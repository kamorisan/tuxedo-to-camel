#
# Makefile for Tuxedo Message Service
#

# Tuxedo Environment
TUXDIR = /u01/oracle/tuxHome/tuxedo22.1.1.0.0
APPDIR = /u01/oracle/user_projects/tuxdemo

# Compiler and flags
CC = gcc
CFLAGS = -I$(TUXDIR)/include -I./src
LDFLAGS = -L$(TUXDIR)/lib
LIBS = -ltux -lbuft -lfml -lfml32 -lengine -lqpid-proton

# Source files
SRC_DIR = src
SRCS = $(SRC_DIR)/MSGSVC.c $(SRC_DIR)/amqclient.c

# Target server
SERVER = MSGSVC

# Build rules
all: $(SERVER)

$(SERVER): $(SRCS)
	buildserver -v -o $(SERVER) -f "$(SRCS)" -l "$(LIBS)"

clean:
	rm -f $(SERVER) *.o core

install: $(SERVER)
	cp $(SERVER) $(APPDIR)/

.PHONY: all clean install
