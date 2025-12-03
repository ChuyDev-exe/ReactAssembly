EMCC = emcc
SRC = ../WebAssembly/simple.c
OUT_DIR = ../public/wasm
OUT_JS = $(OUT_DIR)/simple.js

EM_FLAGS = \
	-s USE_SDL=2 \
	-s USE_SDL_TTF=2 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s WASM=1 \
	-s EXPORTED_FUNCTIONS='["_main"]' \
	-s EXPORTED_RUNTIME_METHODS='["cwrap","ccall"]' \
	-s ENVIRONMENT='web' \
	-s SINGLE_FILE=0 \
	--preload-file fuente.ttf@/fuente.ttf

all: $(OUT_JS)

$(OUT_JS): $(SRC)
	mkdir -p $(OUT_DIR)
	@echo "Compilando: $(SRC) → $(OUT_JS)"
	$(EMCC) $(SRC) -o $(OUT_JS) $(EM_FLAGS)

clean:
	rm -f $(OUT_DIR)/simple.js $(OUT_DIR)/simple.wasm $(OUT_DIR)/simple.data

.PHONY: all clean
