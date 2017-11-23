TARGET ?= a
CC=gcc

SRCS := $(wildcard *.c) 
SRCS += heap-in-a-file/persist.c
HDRS := $(wildcard *.h)
OBJS := $(addsuffix .o,$(basename $(SRCS)))

CPPFLAGS ?= $(INC_FLAGS) -g -O0
LDFLAGS ?= -L/usr/local/lib -lasyncd -lqlibc -pthread -levent -levent_pthreads -lssl -lcrypto -levent_openssl -lmicrohttpd

REPS=25000

all:	res plot.gnu
	gnuplot -p plot.gnu
	gnuplot -p hist.gnu

res:	$(TARGET)
	rm -f theheap; ./$(TARGET) $(REPS) > res

$(TARGET): $(OBJS)
	$(CC)  -o $@  $(OBJS) $(LDFLAGS)

# c source
%.o: %.c forwards.hh
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

forwards.hh:	makeforwards.sh tags
	./makeforwards.sh

tags:	$(SRCS) $(HDRS) heap-in-a-file/persist.c
	ctags -R 

heap-in-a-file/persist.c:
	git clone https://github.com/adrianmay/heap-in-a-file.git

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) forwards.hh core theheap tags res

