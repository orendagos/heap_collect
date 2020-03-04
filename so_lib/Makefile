CC=gcc
src = $(shell find ./ -name "*.c")
deps = $(shell find ./ -name "*.h")
object = $(src:%.c=%.o) 

CFLAGS+=-Wall -Wl,--wrap=cust_malloc -Wl,--wrap=cust_free
# CFLAGS+=-DPRINT_LOG=printf
target = libheap_ana.so 
$(target):$(deps)
	# gcc test_a.c test_b.c -fPIC -shared -o libtest.so
	$(CC) $(src) -fPIC -shared -o $(target)
.PHONY : clean
clean :
	-rm -f $(target) $(object) *so