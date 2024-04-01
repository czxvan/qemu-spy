SRC_PATH=/home/czx/qemu-workspace/qemu-spy/plugin_spy
TOP_SRC_PATH = $(SRC_PATH)/..
PKG_CONFIG=pkg-config
CC=cc -m64 -mcx16
CFLAGS=-O2 -g

CFLAGS += $(shell $(PKG_CONFIG) --cflags glib-2.0)
CFLAGS += -fPIC -Wall
CFLAGS += -I$(TOP_SRC_PATH)/include
CFLAGS += -I$(TOP_SRC_PATH)/include/qemu
CFLAGS += -I$(TOP_SRC_PATH)/host/include/generic

all: $(SONAMES)
.SUFFIXES:
%.o: %.c
	@$(CC) $(CFLAGS) $(PLUGIN_CFLAGS) -c -o build/$@ $<
	@echo "${<}-->${@}"


%: %.o
	@$(CC) -shared -o build/lib$@.so build/$^
	@echo "${<}-->lib${@}.so"

run:
	cd test/ && bash start-qemu-user.sh && cd ..

runsys:
	make aflspy
	cd test/ && bash start-qemu.sh && cd ..

clean:
	rm -f build/*.o build/*.so

.PHONY: all clean
.SECONDARY:
