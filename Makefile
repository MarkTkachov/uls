BIN := uls


BIN_TARGET := ULS

LIBS :=  ./libmx/libmx.a
  

.PHONY = all clean uninstall reinstall

CC = clang -std=c11  -Wall -Wextra -Werror -Wpedantic #compiler -std=c11

SOURCE := $(wildcard src/*.c)

HEAD := $(wildcard inc/*.h)

OBJECTS = $(addprefix obj/,$(notdir $(SOURCE:.c=.o)))

all: $(OBJECTS) $(BIN_TARGET) 

$(BIN_TARGET): $(SOURCE) $(OBJECTS) $(HEAD)
#mkdir -p obj 
# $(CC) -c  $(SOURCE) 
#mv $(OBJECTS) ./obj
	@make -C ./libmx
	@$(CC) -o $(BIN) $(OBJECTS) $(LIBS)

obj/%.o: src/%.c $(HEAD)
	@mkdir -p obj 
	@$(CC) -c $< -o $@

uninstall: clean
	@make -C ./libmx uninstall
	@rm -f $(BIN)

clean:
	@make -C ./libmx clean
	@rm -drf obj
	@rm -f $(wildcard *.o)

reinstall: uninstall $(BIN_TARGET)
