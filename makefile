TARGET   = binary

CC       = gcc
CFLAGS   = -Wall -I.

LINKER   = gcc
LFLAGS   = -I.

SRCDIR   = src
OBJDIR   = obj

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: remove
remove: clean
	@$(rm) $(TARGET)
	@echo "Executable removed!"

.PHONY: clean
clean: remove
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"