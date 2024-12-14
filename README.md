# How to build
```bash
$ make -j  # build the executable
$ make build/assets  # copy the assets
```

# Dependencies
```bash
$ pacman -Sy sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

# Assets
- [Werewolf sprite][player-sprite]
- [Music][music]
- [Sound][sound]
- [Tiles][tiles]
- [Arrow buttons][arrow-buttons]

[player-sprite]: https://opengameart.org/content/werewolf-lpc
[music]: https://opengameart.org/content/caketown-cuteplayful
[sound]: https://opengameart.org/content/foot-walking-step-sounds-on-stone-water-snow-wood-and-dirt
[tiles]: https://opengameart.org/content/simple-platformer-tileset
[arrow-buttons]: https://opengameart.org/content/mobile-ui-buttons-arrows-and-shoot

# Bear
Bear is used to build project & generate `compile_commands.json` for YCM linter:

```bash
$ pacman -Sy bear
$ bear -- make -j
```

# References
- [lazyfoo SDL tutorials][lazyfoo]
- [SDL2 documentation][sdl2-docs]

[lazyfoo]: https://lazyfoo.net/tutorials/SDL/index.php
[sdl2-docs]: https://wiki.libsdl.org/SDL2/CategoryAPI
