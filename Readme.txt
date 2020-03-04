1.so库中的函数没有定义。
CFLAGS+=-Wall -Wl,--wrap=cust_malloc -Wl,--wrap=cust_free
如果没有上一行，编译main的时候，会提示so库没有定义__real_wrap_cust_malloc等。
用objdump -tT *.so | grep "UND"的确会有很多没有定义的函数。这个问题应该不算解决。

2、so%库调用了main_request.h中申明的函数，但是编译的时候，还会提示undefined。



export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/wico_wen/test/heap_stack_collect/code/so_lib

