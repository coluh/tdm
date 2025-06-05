
sources := $(shell find src/ -name "*.c")
objects := $(sources:.c=.o)
target := build/raytdm.out

CFLAGS := -lraylib -lm
CFLAGS += -g -ggdb -rdynamic
CFLAGS += -Werror -Wall

.PHONY: all run clean
all: $(target)

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

$(target): $(objects)
	@mkdir -p build/
	gcc $^ -o $@ $(CFLAGS)

run: $(target)
	./$(target)

clean:
	rm -f $(objects) $(target)
