# Compile Instructions

## Visual Studio

1. Clone or download the repository
1. Open the repo folder in Visual Studio 2022+.
2. Wait for cmake to configure the project and load all assets (this may take a few minutes on the first run).
3. Right click a folder in the solution explorer and switch to the 'CMake Targets View'
4. Select platform and configuration from the dropdown. EG: `Windows64 - Debug` or `Windows64 - Release`
5. Pick the startup project `Minecraft.Client.exe` or `Minecraft.Server.exe` using the debug targets dropdown
6. Build and run the project:
   - `Build > Build Solution` (or `Ctrl+Shift+B`)
   - Start debugging with `F5`.

### Dedicated server debug arguments

- Default debugger arguments for `Minecraft.Server`:
  - `-port 25565 -bind 0.0.0.0 -name DedicatedServer`
- You can override arguments in:
  - `Project Properties > Debugging > Command Arguments`
- `Minecraft.Server` post-build copies only the dedicated-server asset set:
  - `Common/Media/MediaWindows64.arc`
  - `Common/res`
  - `Windows64/GameHDD`

## CMake (Windows x64)

Configure (use your VS Community instance explicitly):

Open `Developer PowerShell for VS` and run:

```powershell
cmake --preset windows64
```

Build Debug:

```powershell
cmake --build --preset windows64-debug --target Minecraft.Client
```

Build Release:

```powershell
cmake --build --preset windows64-release --target Minecraft.Client
```

Build Dedicated Server (Debug):

```powershell
cmake --build --preset windows64-debug --target Minecraft.Server
```

Build Dedicated Server (Release):

```powershell
cmake --build --preset windows64-release --target Minecraft.Server
```

Run executable:

```powershell
cd .\build\windows64\Minecraft.Client\Debug
.\Minecraft.Client.exe
```

Run dedicated server:

```powershell
cd .\build\windows64\Minecraft.Server\Debug
.\Minecraft.Server.exe -port 25565 -bind 0.0.0.0 -name DedicatedServer
```

Notes:
- Post-build asset copy is automatic for `Minecraft.Client` in CMake (Debug and Release variants).
- The game relies on relative paths (for example `Common\Media\...`), so launching from the output directory is required.

## CMake (Linux x64 Cross-Compile with Clang)

Cross-compile Windows x64 binaries on Linux using LLVM/Clang and the Windows SDK obtained via xwin.

### Prerequisites

Install the following packages (example for Ubuntu):

```bash
sudo apt install clang lld llvm cmake ninja-build rsync cargo
```

Install xwin for downloading the Windows SDK:

```bash
cargo install xwin
```

### Compile

Run this (Release):
```bash
./build-linux.sh
```

Or, for debug:
```bash
./build-linux.sh . Debug
```

### NixOS / Nix

For NixOS or systems with Nix installed, use the provided flake:

```bash
nix build .#client
nix build .#server
```

Or enter the development shell with all dependencies:

```bash
nix develop
```

Notes:
- Requires LLVM 15+ with clang-cl, lld-link, llvm-rc, and llvm-mt.
- Wine is required to run the compiled Windows executables on Linux.

