# SkiFree WebAssembly

A WebAssembly port of the classic SkiFree game.

## Prerequisites

- Emscripten SDK (install globally):
  ```bash
  # Clone the emsdk repository
  git clone https://github.com/emscripten-core/emsdk.git
  cd emsdk

  # Install and activate the latest version
  ./emsdk install latest
  ./emsdk activate latest

  # Add to your shell's startup script (e.g., ~/.bashrc or ~/.zshrc)
  echo 'source "$(pwd)/emsdk_env.sh"' >> ~/.bashrc
  source ~/.bashrc
  ```
- CMake 3.13 or higher
- SDL2 (provided by Emscripten)

## Building

The project uses a Makefile for easy building and serving. Run:

```bash
make all
```

This will build the project and copy the output files to the `web` directory.

## Running

To serve the game in your browser, run:

```bash
make serve
```

Then open [http://localhost:8000/skifree.html](http://localhost:8000/skifree.html).

## Hot Reloading

If you have `entr` installed, you can use hot reloading:

```bash
make watch
```

This will automatically rebuild and copy files when you change the source.

## Controls

- Left/Right arrow keys to steer
- Up arrow to speed up
- Down arrow to slow down 