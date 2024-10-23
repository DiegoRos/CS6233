objects = lab7

all: $(objects)

$(objects): %: %.c
	gcc -o $@ $< -pthread
