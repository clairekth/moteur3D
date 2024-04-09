SYSCONF_LINK = g++
CPPFLAGS     = -g
LDFLAGS      = 
LIBS         = -lm

DESTDIR = ./
TARGET  = main
TEST_TARGET = test

# Liste des fichiers sources pour main
MAIN_SOURCES := $(wildcard *.cpp)
MAIN_SOURCES := $(filter-out test.cpp,$(MAIN_SOURCES))  # Exclure test.cpp
MAIN_OBJECTS := $(patsubst %.cpp,%.o,$(MAIN_SOURCES))

# Liste des fichiers sources pour test
TEST_SOURCES := $(wildcard *.cpp)
TEST_SOURCES := $(filter-out main.cpp,$(TEST_SOURCES))  # Exclure main.cpp
TEST_OBJECTS := $(patsubst %.cpp,%.o,$(TEST_SOURCES))

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(MAIN_OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(MAIN_OBJECTS) $(LIBS)

$(DESTDIR)$(TEST_TARGET): $(TEST_OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TEST_TARGET) $(TEST_OBJECTS) $(LIBS)

%.o: %.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

test.o: all  # Faire dépendre test.o de all (ou d'une autre cible indépendante)
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) test.cpp -o $@


clean:
	-rm -f $(MAIN_OBJECTS)
	-rm -f $(TEST_OBJECTS)
	-rm -f $(TARGET)
	-rm -f $(TEST_TARGET)
	-rm -f *.tga

run: $(DESTDIR)$(TARGET)
	$(DESTDIR)$(TARGET)

run_test: $(DESTDIR)$(TEST_TARGET)
	$(DESTDIR)$(TEST_TARGET)