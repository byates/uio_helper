OUTFILE = libuio_helper.a
SOURCES := $(wildcard src/*.c)

APP_BUILD_FLAGS := -std=gnu11 -Wl,-gc-sections -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
APP_COMPILE_FLAGS := -Isrc
APP_COMPILE_FLAGS += -O3 -ggdb3 -Wall -Wno-unknown-pragmas -fmessage-length=0
# -MMD   : output dependencies but leave out system files
# -MP    : Adds dummy rules to works around header file issues during make
# -MF    : Specifies which file to write dependencies to
# -MT    : Change the target of the rule emitted by dependency generation
DEP_FLAGS = -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

OBJ_DIR = obj
OBJS_FILENAMES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

.DEFAULT_GOAL := all
.PHONY: $(OUTFILE)
.PHONY: clean

createDirectories:
	@echo '==> Building target: $(OUTFILE)'
	@mkdir -p $(OBJ_DIR)/src

$(OBJ_DIR)/%.o: %.c
	@echo 'Building file: $<'
	@$(CC) $(APP_BUILD_FLAGS) $(APP_COMPILE_FLAGS) $(DEP_FLAGS) -c -o "$@" "$<"

$(OUTFILE): % : createDirectories $(OBJS_FILENAMES)
	@$(AR) -rcs $(OUTFILE) $(OBJS_FILENAMES)
	@echo 'Finished successfully building: "$(OUTFILE)"'

all: $(OUTFILE)

clean:
	-@rm -rf obj
	-@rm -f $(OUTFILE)
	@echo 'Clean complete for "$(OUTFILE)"'

