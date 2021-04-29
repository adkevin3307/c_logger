EXE_SO = logger.so

OBJ_DIR = obj
TRASH = .cache

SOURCES = $(wildcard *.c)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CFLAGS_SO = -Wall -O2 -shared -fPIC

LIBS = -ldl

all: create_object_directory $(EXE_SO)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS_SO) -c -o $@ $<

$(EXE_SO): $(OBJS)
	$(CC) $(CFLAGS_SO) -o $@ $^ $(LIBS)

clean:
	rm -rf $(EXE_SO) $(OBJ_DIR) $(TRASH)
