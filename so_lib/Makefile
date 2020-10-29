CC=gcc
AR:=ar
LD:=ld
src = $(shell find ./ -name "*.c")
deps = $(shell find ./ -name "*.h")
object = $(src:%.c=%.o) 


CFLAGS+=-Wall -Wl,--wrap=cust_malloc -Wl,--wrap=cust_free -fPIC -g


OBJS := cust_malloc.o signal_chain.o
INCLUDE := -I .
static_lib:$(OBJS)
	$(AR) rcs libheap_collect.a $(OBJS)
	# $(CC) -fPIC -shared $(OBJS) -o $(target)
	$(CC) -shared $(OBJS) -o $(target)
$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

# CFLAGS+=-DPRINT_LOG=printf
target = libheap_ana.so 
$(target):$(deps)
	# gcc test_a.c test_b.c -fPIC -shared -o libtest.so
	$(CC) $(src) -fPIC -shared -o $(target)

.PHONY : clean
clean :
	-rm -f $(target) $(object) *so