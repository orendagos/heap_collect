CC=gcc
# src = $(shell find ./ -name "*.c")
# deps = $(shell find ./ -name "*.h")
src = main.c
deps = main_request.h
object = $(src:%.c=%.o) 
CFLAGS+=-Wall -Wl,--wrap=cust_malloc -Wl,--wrap=cust_free
target = main 
$(target):$(deps)
	$(CC) $(CFLAGS) $(src) -L./so_lib/ -lheap_ana -o $(target)
.PHONY : clean
clean :
	-rm -f $(target) $(object)