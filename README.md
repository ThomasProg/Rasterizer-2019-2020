# rasterizer

## Features :

- You can move by pressing wasd keys, and rotate your camera with the mouse or ijkl.
- Press F1 to show the wireframe mode.
- Press F2 to teleport set light to your location.

- Perspective
- .obj loadable

## Known issues : 
 - When a big triangle is inside you, even though there is a large surface of it forward, it will be culled if one vertex goes behind.

 - WaddleDee Texture bug in media/WaddleDeeLow-Poly/waddledee.obj in vertex : 
    v 19.888699 61.303619 -103.692482 (and those around)
because of negative uvs.

- Boo transparence not very good : we can still see its insides ; which is voluntary for the moment. 
To fix it, we have to apply the depth buffer for the same entity. However, there would be too much function parameters, and it would go outside of the project scope.

- Boo texture doesn't have normals in its obj : default normals set to (0,0,1) for every vertex.

- Low frame rate when a lot of pixels are drawn on screen.

- Different interpolations not available on the same run; to fix that, we could replace macros if variables and pass them throughout the code, but it is not necessary for now.