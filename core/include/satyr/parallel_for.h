#pragma once

#ifdef SATYR_WITH_TBB
#include <satyr/tbb_for.h>
#else
#include <satyr/fallback_parallel_for.h>
#endif
