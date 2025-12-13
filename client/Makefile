CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../engine -I../dependencies/glm/include
CFLAGS = -Wall -std=c++20 -fexceptions
RCFLAGS = 
RESINC = 
LIBDIR = 
LIB = -lengine
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -D_DEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR) -L../engine/bin/Debug
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/client

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR) -L../engine/bin/Release
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s -Wl,-rpath='$$ORIGIN'
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/client

# --- AUTO-DISCOVERY DEI TEST ---
# Trova tutti i file che finiscono per Test.cpp (es. HanoiTest.cpp)
TEST_SRCS = $(wildcard *Test.cpp)

# Crea la lista degli oggetti dei test per Debug e Release
TEST_OBJS_DEBUG = $(patsubst %.cpp,$(OBJDIR_DEBUG)/%.o,$(TEST_SRCS))
TEST_OBJS_RELEASE = $(patsubst %.cpp,$(OBJDIR_RELEASE)/%.o,$(TEST_SRCS))

# Aggiunge gli oggetti dei test alla lista principale
# Nota: Ho corretto il percorso di OBJ_DEBUG che prima puntava erroneamente a Release
OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/hanoi.o $(TEST_OBJS_DEBUG)
OBJ_RELEASE = $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/hanoi.o $(TEST_OBJS_RELEASE)

all: debug release

# --- NUOVO TARGET TEST ---
# Esegue la build di debug e poi lancia l'eseguibile.
# Se l'eseguibile ritorna un errore (exit code != 0), make fallisce.
test: debug
	@echo "--- Esecuzione Test ---"
	./$(OUT_DEBUG)

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

# Regole specifiche esistenti
$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

# --- REGOLA GENERICA PER I TEST IN DEBUG ---
# Compila automaticamente qualsiasi file *Test.cpp trovato
$(OBJDIR_DEBUG)/%Test.o: %Test.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $< -o $@

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

# Regole specifiche esistenti
$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c hanoi.cpp -o $(OBJDIR_RELEASE)/hanoi.o

# --- REGOLA GENERICA PER I TEST IN RELEASE ---
$(OBJDIR_RELEASE)/%Test.o: %Test.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $< -o $@

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release test