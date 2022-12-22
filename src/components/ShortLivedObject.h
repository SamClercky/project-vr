#ifndef PROJECT_VR_SHORTLIVEDOBJECT_H
#define PROJECT_VR_SHORTLIVEDOBJECT_H

#include <cstdint>
#include "components/DeltaTime.h"

namespace components {
    struct ShortLivedObject {
        uint64_t time_to_live; // in ms

        explicit ShortLivedObject(float ttl_seconds): time_to_live((uint64_t)(ttl_seconds * 1000.f)) {}
        explicit ShortLivedObject(uint64_t ttl_ms): time_to_live(ttl_ms) {}

        ShortLivedObject& operator +=(DeltaTime dt) { time_to_live += dt.dt; return *this; }
        ShortLivedObject& operator -=(DeltaTime dt) { time_to_live -= dt.dt; return *this; }
        ShortLivedObject& operator +=(uint64_t dt_ms) { time_to_live += dt_ms; return *this; }
        ShortLivedObject& operator -=(uint64_t dt_ms) { time_to_live -= dt_ms; return *this; }
        ShortLivedObject& operator +=(float dt_sec) { time_to_live += (uint64_t)(dt_sec * 1000.f); return *this; }
        ShortLivedObject& operator -=(float dt_sec) { time_to_live -= (uint64_t)(dt_sec * 1000.f); return *this; }
    };
}

#endif//PROJECT_VR_SHORTLIVEDOBJECT_H
