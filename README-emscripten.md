# How to build
Quite straightforwards as SDL2 and its modules (ttf, image, mixer) were already ported by Emscripten:

```terminal
$ source <emsdk-dir>/emsdk_env.sh
$ cd <sdl-project>
$ mkdir build-emscripten && cd build-emscripten 
$ cp -r assets/ build-emscripten/
$ emcc -std=c++20 -I../include ../src/* -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=[png] -s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS=[mp3] --preload-file assets -o index.html
```

# How to run
```terminal
$ python -m http.server 8000
```

# Useful links
- To install emscripten, refer to [its offical website][install-emscripten]
- For porting a game made with SDL2 to emscripten see [official tutorial][sdl-emscripten].
- To link with ported SDL and its modules, refer to this [question on stackoverflow][stackoverflow-thread].

[sdl-emscripten]: https://wiki.libsdl.org/SDL2/README/emscripten
[install-emscripten]: https://emscripten.org/docs/getting_started/downloads.html
[stackoverflow-thread]: https://stackoverflow.com/a/43030517/2228912

# Notions
## Emscripten
Used to compile C/C++ code to WebAssembly (to run on browser).

### Infinite loop (game loop)
- Problematic in browser, as there's no way to return control to browser so that:
  - Other events can be processed.
  - Other code can be run (e.g. JS API like WebGL).

## WebAssembly
- Binary code that runs on a virtual machine within the web browser.
- Provides near-native execution speeds.
