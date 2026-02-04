CC = gcc
CFLAGS = -g -DSTR_IMPLEMENTATION

STR_H = ../str.h
BUILD = build

LIFECYCLE = lifecycle
TRIMS = trims
APPENDS = appends
CHARS = chars

.PHONY: all clean
all: $(BUILD) $(LIFECYCLE) $(APPENDS) $(TRIMS) $(CHARS)

$(BUILD):
	mkdir -p $(BUILD)

$(LIFECYCLE) $(CHARS) $(TRIMS) $(APPENDS): %: %.c $(STR_H) | $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/$@ $<

clean:
	rm -rf $(BUILD)

