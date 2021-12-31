# Project Setup

```
git clone https://github.com/krogenth/AdditionalPylons.git
cd AdditionalPylons
git submodule update --init --recursive
./build.ps1
```

All builds can be found within `./build`

# Execution

You will need the Chaoslauncher, a copy can be found within `./libs/bwapi/Release_Binary/Chaoslauncher/`
Set up `bwapi.ini` within `%starcraft_dir%/bwapi-data/`, specifically:
```
auto_Menu = SINGLE_PLAYER
map = maps/BroodWar/sscai/(?)*.sc
game_type = MELEE
```
Run the built `AdditionalPylons_client.exe` in `./build/Release/`.
Through Chaoslauncher, tick the BWAPI Injector(Release or Debug, depending on build used), then press start.
The game should start and auto navigate the menu to start the game, and the bot client should connect once the game starts.
