# Space-Rider
![Screengrab](https://user-images.githubusercontent.com/48212096/173443882-ab8b61fc-fcae-4d44-a014-227d4b1402da.png)
A Computer Graphics project in OpenGL created as part of a University project.

The theme for this project was space and 1980s retro wave. The objective for the player is to collect all coins and not die to enemies.

# Features
The graphical effects implemented in this project are as follows:
  - Omni-directional shadows (casted from the sun).
  - Bloom (used Call of Duty: Advanced Warfare Presentation for implementation).
  - Multi-Texturing (used to map diffuse and emissive textures onto models).
  - Instanced Rendering.
  - Simplex Noise (used for vertex displacement).
  - Skybox
  - Environment mapping (used on track).

# Controls
The player takes control of a flying car which they can move side-to-side on the track in order to avoid enemies or collect items:
  - **A/D**	- Strafe Left/Right
  - **Mouse Left Click** - Shoot Bullet
  - **1/2/3/4**	- First Person, Top View, Third Person, Freecam
  - **Z (Hold)** - Speed up/Slow down game
  - **G/H** - Turn on Godmode/Turn off Godmode
  - **(In Freecam) W A S D** - Move camera
  - **(In Freecam) Mouse Move** - Look around

# Build Instructions
Visual Studio 2022 is required for this project. Older visual studio version might work but I have only tested it with VS2022 so there is no guarentee. You can run in either Release or Debug mode. Release mode is recommended for best performance.

Open SpaceRider.sln and click Run Without Debugger.

# Screenshots
## Bloom
![Sun](https://user-images.githubusercontent.com/48212096/173443906-1a8c26e7-f8e7-497d-800b-c75c7c4543f7.png)
## Environment Mapping
![Environment Mapping](https://user-images.githubusercontent.com/48212096/173443942-2d1984a0-6b8e-4889-8761-019bde02330f.png)
## Omni-directional/Point shadows
![Shadow Mapping](https://user-images.githubusercontent.com/48212096/173443947-65e0ee69-30a0-458f-bb6e-e59e957d7661.png)
## Instanced Rendering
![Instanced Asteroids](https://user-images.githubusercontent.com/48212096/173443957-1ec90e3e-f2d9-4537-b31f-b19a71a89d0d.png)
## Multi-Texturing (Diffuse + Emissive)
![Car](https://user-images.githubusercontent.com/48212096/173444393-2b33c6b7-4eac-465d-ac48-378a4004ce48.png)
![Enemy](https://user-images.githubusercontent.com/48212096/173444405-ac20847a-a5dd-47e8-a164-d82686af8653.png)
![StreetLamp](https://user-images.githubusercontent.com/48212096/173443976-3a5e696b-32ba-43b9-8c51-c2f2def41aa3.png)
## Octasphere primitive (texture mapping)
![OctaSphere - Texture Mapping](https://user-images.githubusercontent.com/48212096/173443996-7173029c-f1ca-40d5-8058-87763d6013f4.png)

# Assets
  - **Street Light:** https://sketchfab.com/3d-models/street-light-2d44b333f56c4faaab1bd2007868cc37
  - **Skybox:** https://opengameart.org/content/space-skybox-1
  - **Player Car:** https://sketchfab.com/3d-models/draftpunk-coursework-cyberpunk-car-a6d8bf6728484dcea0054e9235c2bcf0
  - **Enemy Drone:** https://sketchfab.com/3d-models/drone-ad0e1d85295847409e961de33c2ee0bf
  - **Planet Textures:** https://www.solarsystemscope.com/textures/
  - **Asteroids:** https://sketchfab.com/3d-models/gray-rock-e090fe6e5b2248ef928d49a04c67905f, https://sketchfab.com/3d-models/gray-lake-rock-efc76e8b35c847d78c1ba0253806bc55
  - **Coin Texture:** https://opengameart.org/node/7328
  - **Health Texture:** https://opengameart.org/content/green-gradient-fabric
  - **Next-Generation Post Processing in Call of Duty: Advanced Warfare Presentation (Used for bloom shader):** http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
  - **Code - Bloom Shader Reference (Unity and Blender)**: https://github.com/Unity-Technologies/Graphics/blob/master/com.unity.postprocessing/PostProcessing/Shaders/Builtins/Bloom.shader , https://github.com/blender/blender/blob/master/source/blender/draw/engines/eevee/shaders/effect_bloom_frag.glsl
  - **Code - Track Grid Pattern:** https://gamedev.stackexchange.com/questions/174380/how-to-decorate-the-floor-with-a-grid
  - **Code - Track Multipoint Color Gradient:** https://www.shadertoy.com/view/3tGBWt
  - **Code - Cos Palette:** https://iquilezles.org/www/articles/palettes/palettes.htm
  - **Code - Shader Simplex Noise:** https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
