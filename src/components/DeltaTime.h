//
// Created by decle on 11/29/2022.
//

#ifndef OPENGL_TEST2_DELTATIME_H
#define OPENGL_TEST2_DELTATIME_H

#include <cstdint>

namespace components {

struct DeltaTime {
  uint64_t dt;

  explicit DeltaTime(uint64_t dt) : dt(dt) {}
};

} // namespace components

#endif // OPENGL_TEST2_DELTATIME_H