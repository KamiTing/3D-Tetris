# 3D Tetris

A minimal 3D Tetris game implemented in C++ using OpenGL and FreeGLUT.

## 🎮 Features

- 3D Tetris gameplay with gravity and layer clearing
- Rotatable blocks on all three axes (X/Y/Z)
- Projection-aligned block preview (Ghost block)
- Hold & Next block system
- Randomized block generation with fair distribution
- Minimalist rendering using OpenGL primitives (no textures)

## 🧱 Block Types

Includes all 7 standard Tetris blocks in 3D, rendered using `{x, y, z}` coordinates.

## 🖥️ Requirements

- C++17 compiler (e.g. g++ 12.1.0)
- OpenGL
- FreeGLUT
- CMake (optional, for build configuration)

## 🔧 Build Instructions (Windows + VS Code)

1. Clone this repository:
   ```bash
   git clone https://github.com/KamiTing/3D-Tetris.git
   ```

2. Make sure you have `g++`, `FreeGLUT`, and required headers (like `stb_image.h`) installed.

3. Compile with:
   ```bash
   g++ -std=c++17 -Iinclude -Llib src/*.cpp -lfreeglut -lopengl32 -o 3DTetris.exe
   ```

4. Run the game:
   ```bash
   ./3DTetris.exe
   ```

## 📁 Project Structure

```
3DTetris/
├── src/               # C++ source files
├── include/           # Header files (GL, stb_image, etc.)
├── shaders/           # GLSL shaders (if applicable)
├── .vscode/           # VS Code settings (optional)
├── README.md          # Project description
└── .gitignore         # Git ignore rules
```

## ✨ To Do

- Sound effects and background music
- High score and game-over screen
- Configurable controls
- Level progression with speed-up

## 🧑‍💻 Author

Po-Ting Chiang (`KamiTing`)

## 📜 License

MIT License. See `LICENSE` for details.
