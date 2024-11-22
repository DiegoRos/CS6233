objects = lab9_c_producer lab9_c_consumer

all: $(objects)
	@echo "Usage: \"./lab9_c_producer <n>\" in one terminal window, and \"./lab9_c_consumer <n>\" in another."

$(objects): %: %.c
	gcc -o $@ $< -lrt
