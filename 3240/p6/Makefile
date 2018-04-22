TARGETS = p6.so p6

all: $(TARGETS)

p6.so: timestamp.c
	gcc -shared -fpic -o p6.so timestamp.c
p6: p6.c 
	gcc -rdynamic -o p6 p6.c -ldl
clean:
	rm -f $(TARGETS)
