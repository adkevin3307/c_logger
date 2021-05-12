EXE = logger
EXE_SO = logger.so

OBJ_DIR = obj
TRASH = .cache

SOURCES = main.c
SOURCES_SO = logger.c

OBJS = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))
OBJS_SO = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(notdir $(SOURCES_SO))))

CFLAGS = -Wall -O2
CFLAGS_SO = -shared -fPIC

LIBS =
LIBS_SO = -ldl

all: create_object_directory $(EXE) $(EXE_SO)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/main.o: $(SOURCES)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/logger.o: $(SOURCES_SO)
	$(CC) $(CFLAGS) $(CFLAGS_SO) -c -o $@ $<

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(EXE_SO): $(OBJS_SO)
	$(CC) $(CFLAGS) $(CFLAGS_SO) -o $@ $^ $(LIBS_SO)

clean:
	rm -rf $(EXE) $(EXE_SO) $(OBJ_DIR) $(TRASH)
