CC = gcc

CFLAGS = -Wall

LDFLAGS = -lm

program = wall

source = \
	 wallBuilder.c \

obj = $(source:.c=.o)

$(program): $(obj) wallHeader.h
	        $(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

%.o: %.c
	        $(CC) $(CFLAGS) -c $< -o $@

clean:
	        rm -f $(program) $(obj)
run:
	        ./$(program)
edit:
	        vim -p $(source) wallHeader.h
