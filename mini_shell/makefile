TARGET := ../lib/libshell.so
SRCS := $(wildcard *.c)
CFLG := -I ../include/

${TARGET} : ${SRCS}
	gcc -fPIC -shared ${CFLG} $^ -o $@

clean :
	rm ${TARGET}
