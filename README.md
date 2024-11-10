# How to build
```bash
$ make -j  # build the executable
$ make build/assets  # copy the assets
```

# Dependencies
```bash
$ pacman -Sy sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

# Bear
Bear is used to build project & generate `compile_commands.json` for YCM linter:

```bash
$ pacman -Sy bear
$ bear -- make -j
```

# References
[lazyfoo SDL tutorials][1]

[1]: https://lazyfoo.net/tutorials/SDL/index.php
