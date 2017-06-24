#pragma once

#include <satyr/config.h>

#ifdef SATYR_WITH_JEMALLOC
#include <satyr/n_array/jemalloc_allocation.h>
#else
#include <satyr/n_array/standard_allocation.h>
#endif
