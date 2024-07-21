CC = g++
CFLAGS = -std=c++17 -Wall
SRCDIR = ./src
BINDIR = ./bin


SRC := weedsim.cpp experiments.cpp

SRCS_CLI := $(SRC) cli_prototype.cpp  # Add your source files here
TARGET_CLI := weedsim_cli  # Change 'myprogram' to your desired executable name

SRCS_GUI := $(SRC) gui_prototype.cpp  # Add your source files here
TARGET_GUI := weedsim_GUI  # Change 'myprogram' to your desired executable name

OBJS_CLI := $(patsubst %.cpp,$(BINDIR)/%.o,$(SRCS_CLI))
OBJS_GUI := $(patsubst %.cpp,$(BINDIR)/%.o,$(SRCS_GUI))

GUI_FLAGS = -lpng -lstdc++fs -lGL -lX11 -lpthread -lfreeimage

all: $(TARGET_CLI) gui

$(TARGET_CLI): $(OBJS_CLI)
	$(CC) $(CFLAGS) -o $@ $^

gui: $(TARGET_GUI)

$(TARGET_GUI): $(OBJS_GUI)
	$(CC) $(CFLAGS) -o $@ $^ $(GUI_FLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_CLI) $(TARGET_CLI) $(OBJS_GUI) $(TARGET_GUI)

zip:
	zip -r 07_xnekul04.zip bin src Makefile README.md doc.pdf graphs.py

run:
	./$(TARGET_CLI)