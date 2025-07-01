#pragma once

#include <cstddef>

namespace hazard {

static constexpr size_t kNumHazardPtrsPerThread = 2;
// Since std::thread::hardware_concurrency() is not constexpr, we use 4 as last
// multiplier.
static constexpr size_t kRetireBatchSize = 2 * kNumHazardPtrsPerThread * 4;

}  // namespace hazard
