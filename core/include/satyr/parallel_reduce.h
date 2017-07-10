#pragma once

#ifdef SATYR_WITH_TBB
#include <satyr/tbb_reduce.h>
#else
#include <satyr/fallback_parallel_reduce.h>
#endif

