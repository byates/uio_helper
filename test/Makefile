
PROJECT_ROOT = ..
EXECUTABLE = test_uio_helper
SOURCES := test.c

APP_LIBS := $(PROJECT_ROOT)/libuio_helper.a
APP_COMPILE_FLAGS := -std=c11
APP_COMPILE_FLAGS += -O0 -g0 -Wall -Wno-unknown-pragmas
APP_COMPILE_FLAGS += -I$(PROJECT_ROOT)/src -I$(COMPILER_LOC)/include

.DEFAULT_GOAL := all

.PHONY: $(EXECUTABLE)
.PHONY: clean

$(EXECUTABLE):
	$(CC) $(APP_COMPILE_FLAGS) -o $(EXECUTABLE) $(SOURCES) $(APP_LIBS)
	@echo 'Finished successfully building: "$(EXECUTABLE)"'
	@echo ' '

all: $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
	@echo 'Clean complete for "$(EXECUTABLE)"'

