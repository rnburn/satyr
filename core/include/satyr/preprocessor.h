#pragma once

#define SATYR_STRINGIFY_IMPL(x) #x
#define SATYR_STRINGIFY(x) SATYR_STRINGIFY_IMPL(x)

#define SATYR_CONCATENATE_IMPL(S1, S2) S1##S2
#define SATYR_CONCATENATE(S1, S2) SATYR_CONCATENATE_IMPL(S1, S2)