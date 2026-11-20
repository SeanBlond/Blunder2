add_library(
    SMATH STATIC

    # Math - Functions
    "src/clamping.h"
    "src/conversions.h"
    "src/trig.h"

    # Math - Vectors
    "src/vectors.h" "src/vectors.cpp"
    "src/ivectors.h" "src/ivectors.cpp"

    # Math - Matrices
    "src/mat4.h" "src/mat4.cpp"

    # Math - Tansforms
    "src/transform.h" "src/transform2D.h"
)
target_include_directories(SMATH PUBLIC include)