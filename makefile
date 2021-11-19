# ------------------------------------Options-----------------------------------
LXXFLAGS = $(shell pkg-config --libs sfml-all) $(shell pkg-config --libs opengl) $(TMP)
TMP = -g -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr
CXXFLAGS = -std=c++17 -O3 $(shell pkg-config --cflags sfml-all) $(shell pkg-config --cflags opengl) -w $(TMP)

# ------------------------------------Options-----------------------------------

# -------------------------------------Files------------------------------------
SrcDir  = src


BinDir = bin
IntDir = $(BinDir)/obj
Deps = $(wildcard $(SrcDir)/*.hpp)


CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))

Objs = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))
# -------------------------------------Files------------------------------------

# ----------------------------------Make rules----------------------------------
.PHONY: all
all: $(BinDir)/editor

$(BinDir)/editor: $(Objs) $(Deps)
	$(CXX) -o $(BinDir)/editor $(Objs) $(LXXFLAGS)

vpath %.cpp $(SrcDir) $(DemoDir) $(TestDir)
$(IntDir)/%.o: %.cpp $(Deps)
	$(CXX) -c $< $(CXXFLAGS) -o $@

.PHONY: init
init:
	mkdir -p bin/obj

.PHONY: clean
clean:
	rm -f $(Objs) $(BinDir)/editor 
# ----------------------------------Make rules----------------------------------