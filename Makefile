SRC_DIR = src
OBJ_DIR = obj

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LIBS = GL GLU glut
FRAMEWORKS =
endif
ifeq ($(UNAME_S),Darwin)
LIBS =
FRAMEWORKS = OpenGL GLUT
endif
INCS = $(SRC_DIR) third_party/glm
DEFS = GL_GLEXT_PROTOTYPES
WARN = all extra

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
DEPS = $(shell find $(SRC_DIR) -name "*.h")
OBJS = $(patsubst $(SRC_DIR)/%.o,$(OBJ_DIR)/%.o,$(SRCS:.cpp=.o))
SRC_SUB_DIRS = $(shell find $(SRC_DIR) -type d)
OBJ_SUB_DIRS = $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SRC_SUB_DIRS))
EXEC = $(shell basename `pwd`)

CXX = g++
CFLAGS = -pipe $(foreach d,$(DEFS),-D$d) $(foreach d,$(INCS),-I$d) $(foreach d,$(WARN),-W$d)
LD = g++
LDFLAGS =

.PHONY: all run debug release profile prepare clean remove

all: debug
run: all
debug: CFLAGS += -g
debug: DEFS += DEBUG
release: CFLAGS += -Ofast
release: DEFS += NDEBUG
profile: CFLAGS += -g -pg -Ofast
profile: LDFLAGS += -pg
profile: DEFS += NDEBUG

debug release profile: $(EXEC)

run: $(EXEC)
	./$(EXEC)

prepare:
	mkdir -p $(OBJ_SUB_DIRS)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_SUB_DIRS)

remove: clean
	rm -f $(EXEC)

$(EXEC): prepare $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(foreach l,$(LIBS),-l$l) $(foreach f,$(FRAMEWORKS),-framework $f)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
