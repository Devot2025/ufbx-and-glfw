# ufbx-and-glfw
This is a program that displays FBX files using ufbx, OpenGL, GLFW, and GLAD.

## Known Limitations and Notes

- **Skinned Meshes:** Supported for rendering in a static/bind pose. **Rigged animation playback is not yet supported.**
- **No-Animation Case:** Projects without animation clips can **glitch** with the current pipeline.  
  Please adjust your shaders / update loop to:
  - Use identity matrices for missing bones,
  - Bypass skinning updates when there are no active animations,
  - Add null checks for empty animation data.
- **Shape Keys (Morph Targets):** Currently **unsupported**.


# Example test image
![Test Image](image/test_a_fbx_gl.png)
