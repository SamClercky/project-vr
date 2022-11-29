#ifndef OPENGL_TEST2_ROTATEANIMATION_H
#define OPENGL_TEST2_ROTATEANIMATION_H

namespace components {

struct RotateAnimation {
  float rotationSpeed;

  explicit RotateAnimation(float speed) : rotationSpeed(speed) {}
};

} // namespace components

#endif // OPENGL_TEST2_ROTATEANIMATION_H
