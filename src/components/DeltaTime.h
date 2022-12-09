#ifndef OPENGL_TEST2_DELTATIME_H
#define OPENGL_TEST2_DELTATIME_H

#include <cstdint>

namespace components {

    struct DeltaTime {
        uint64_t dt;

        explicit DeltaTime(uint64_t dt) : dt(dt) {}
        float sec() const { return (float) dt / 1000.f; }
    };

}// namespace components

#endif// OPENGL_TEST2_DELTATIME_H
