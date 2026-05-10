![HellishEnds Banner](https://media.discordapp.net/attachments/1479684063018352652/1501969775969042443/hellishEndsTitle.png?ex=69fe01db&is=69fcb05b&hm=4491c8121025dc66381aefcc60cedd228869c15be242e4225351f4ec2764385b&=&format=webp&quality=lossless&width=1505&height=284)

## Controls (Keyboard & Mouse)

- **Movement**: `W` `A` `S` `D`
- **Jump / Fly (Up)**: `Space`
- **Sneak / Fly (Down)**: `Shift` (Hold)
- **Sprint**: `Q` (Hold) or Double-tap `W`
- **Inventory**: `E`
- **Chat**: `T`
- **Zoom**: `V`
- **Drop Item**: `B`
- **Crafting**: `C` Use `Q` and `E` to move through tabs (cycles Left/Right)
- **Toggle View (FPS/TPS)**: `F5`
- **Fullscreen**: `F11`
- **Pause Menu**: `Esc`
- **Attack / Destroy**: `Left Click`
- **Use / Place**: `Right Click`
- **Pick Block**: `Middle Click`
- **Select Item**: `Mouse Wheel` or keys `1` to `9`
- **Accept or Decline Tutorial hints**: `Enter` to accept and `B` to decline
- **Game Info (Player list and Host Options)**: `TAB`
- **Toggle HUD**: `F1`
- **Toggle Debug Info**: `F3`
- **Toggle Advanced Tooltips**: `F3` + `H`
- **Open Debug Overlay**: `F4`
- **Toggle Debug Console**: `F6`

## Build & Run

1. Install [Visual Studio 2022](https://aka.ms/vs/17/release/vs_community.exe) or [newer](https://visualstudio.microsoft.com/downloads/).
2. Clone the repository.
3. Open the project folder from Visual Studio.
4. Set the build configuration to **Windows64 - Debug** (Release is also ok but missing some debug features), then build and run.

### CMake (Windows x64)

```powershell
cmake --preset windows64
cmake --build --preset windows64-debug --target Minecraft.Client
```
