CC=gcc
# src = $(shell find ./ -name "*.c")
# deps = $(shell find ./ -name "*.h")
src = main.c
deps = main_request.h
object = $(src:%.c=%.o) 
CFLAGS+=-Wall -Wl,--wrap=cust_malloc -Wl,--wrap=cust_free -g
target = main 
static_test:$(deps) static_lib
	$(CC) $(CFLAGS) $(src) -lheap_collect -L ./so_lib -o main_static_test

static_lib:
	cd so_lib;make -f Makefile static_lib;cd -

$(target):$(deps)
	$(CC) $(CFLAGS) $(src) -L./so_lib/ -lheap_ana -o $(target)


.PHONY : clean
clean :
	-rm -f $(target) $(object)