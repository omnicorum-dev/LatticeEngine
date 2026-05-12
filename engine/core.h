//
// Created by Nico Russo on 5/12/26.
//

#ifndef LATTICEENGINE_CORE_H
#define LATTICEENGINE_CORE_H

#include <iostream>
#include <ostream>
#include <fstream>
#include <csignal>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vendor/other/stb_image_write.h"
#include "vendor/other/stb_image.h"

#include "vendor/nopp/nopp.h"

#define BIT(x) (1 << x)

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define DEBUG_BREAK() __builtin_trap()
#else
    #include <csignal>
    #define DEBUG_BREAK() raise(SIGTRAP)
#endif

#ifdef NDEBUG
    #define ASSERT(x) ((void)0)
#else
    #define ASSERT(x)                                                     \
    do {                                                              \
    if (!(x)) {                                                   \
    LOG_ERROR("\nAssertion failed: {}", #x);                  \
    DEBUG_BREAK();                                            \
    }                                                             \
    } while (0)
#endif

#endif //LATTICEENGINE_CORE_H
