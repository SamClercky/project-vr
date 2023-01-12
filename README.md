# Project VR

## 1. Configured dependencies
* GLFW 3.3-stable
* GLM 0.9.8
* GLAD core profile, no extensions
* stb_image
* EnTT
* Assimp
* Bullet3

## 2. Building the project
Cloning this repository:
```bash
$> git clone --recurse-submodules <name of repo>
```

If you forgot `--recursive-submodules` or want to update the 
dependencies:
```
$> git submodule update --init --recursive
```

To build the project with no help from any IDE:
```bash
$> mkdir out
$> cmake . -B out
$> cmake --build out
```

## 3. Project description

The project is build around the ECS pattern. This means that most game logic is separated 
in 3 parts:
* Creation of entities in the prefabs
* The storage of properties in the components
* A way to make progress and manipulate these entities in the systems

Further there is a scene to combine the ECS and control when the systems get updated and the 
engine code which takes commands from the systems and puts the information in a window on screen.

Notable features:
* Input processing: gamepad or keyboard + mouse implemented with glfw
* Automatic resizing: game screen updates when its surrounding window changes size (glViewport + updated framebuffer textures).
* Skybox: to give a sense of orientation in the scene
* Particle effect (moving snow whirl): based on perlin noise texture and created with a geometry shader which becomes periodically transparent (glEnable(GL\_BLEND) + fragment shader).
* Collision detection: bullet3 + gravity
    * Mode for viewing bounding boxes from bullet3 created with a geometry shader
    * The camera is bound to a collision box, so the player is bound to the physics in the world.
    * Free-cam mode in which the player can detach itself from collision detection boundaries.
* Bullets: can be thrown and collide with other objects (fun to move things around with).
* Implemented light equations (phong shading): containing with attenuation and decreasing intensity with increasing distance:
    * Ambient light
    * Diffuse light
    * Specular light
* Directional shadows: for both point- and spotlights with framebuffers where the scene is rerendered for every light with the possibility to exclude certain objects that do not cast a shadow.
* Framebuffers postprocessing effect: periodically applies an edge detecting kernel function and adds a cross to help with aiming and uses inverted colors of the current frame for better visibilty of the cross.
* Ability to efficiently load and use external resources: by storing the path together with the requested resource, so duplicate calls return the same shared reference to the result.
* Ephemeral objects: Bullets can become to much in a scene and degrade the performance, so a system was created to remove objects if they fall out of the world (y < -100). Additionally a system of ephimeral objects was created.  These objects have a time to live (ttl) after which, they are destroyed.
* Ability to work with transparent (RGBA) and non-transparent (RGB) images and show them correctly
* Gamma correction in a separate branch `gamma_correction`
