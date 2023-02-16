CC=clang
OBJDUMP=llvm-objdump

ODFLAGS+=--triple=aarch64-linux-gnu
ODFLAGS+=-S

CFLAGS+=--target=aarch64-linux-gnu
# You need to download the sysroot tar ball from Linaro website and change this accodingly
CFLAGS+=--sysroot=/home/carlos/disk/sysroot/sysroot-glibc-linaro-2.25-2019.12-aarch64-linux-gnu/
CFLAGS+=-O1
CFLAGS+=-g
# CFI for indirect calls only, you can also use cfi which enable other CFI related protections
CFLAGS+=-fsanitize=cfi-icall
# Need it to enable CFI for indirect calls
CFLAGS+=-flto

SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
BCS:=$(SRCS:.c=.bc)
IR:=$(SRCS:.c=.ll)

all: cfi
cfi: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o cfi

all-bc: $(BCS)

all-ir: $(IR)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.c
	$(CC) $(CFLAGS) -S $<

%.bc: %.c
	$(CC) $(CFLAGS) -emit-llvm -c $<

%.ll: %.bc
	$(BCDIS) $<

dumpcfi: cfi
	$(OBJDUMP) $(ODFLAGS) $<

dump%: %.o
	$(OBJDUMP) $(ODFLAGS) $<

.PHONY: clean
clean:
	rm -f *.o *.bc *.ll cfi
