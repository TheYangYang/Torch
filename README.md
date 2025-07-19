# Torch Engine

**Torch** is a real-time 3D rendering engine built with **C++17** and **OpenGL**.  
It supports advanced physically-based rendering (PBR) techniques and modern real-time visual effects.

---

## ✨ Features

- Physically Based Rendering (PBR)
- Real-time Image-Based Lighting (IBL)
- Physical Bloom (based on SIGGRAPH 2014 Call of Duty presentation)
- Atmospheric Scattering
- Cascaded Shadow Maps (CSM)
- Percentage Closer Soft Shadows (PCSS)
- Screen Space Ambient Occlusion (SSAO)

---

## 🔧 How to Build

### ✅ Prerequisites

- [CMake](https://cmake.org/)
- [Vulkan SDK](https://vulkan.lunarg.com/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/)

### 🧱 Build Instructions

```bash
# Clone the repository with all submodules
git clone --recurse-submodules https://github.com/TheYangYang/Torch.git

# Create a build directory
mkdir build
cd build

# Generate project files
cmake ..

# Build the project
cmake --build .
```

# 💡 Notes
- Requires a GPU that supports Vulkan and OpenGL 4.5+.
- Windows is currently supported. Linux support is experimental.
