/*
 * Copyright (c) 2015, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY LOG OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GEOPM_POLICY_MESSAGE_H_INCLUDE
#define GEOPM_POLICY_MESSAGE_H_INCLUDE
#include <pthread.h>
#include <stdint.h>

#include "geopm_time.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GEOPM_CONST_MAX_NUM_CPU 768
#define GEOPM_GLOBAL_POLICY_IDENTIFIER 0

/// @brief Encapsulates power policy information as a
/// 32-bit bitmask.
enum geopm_policy_flags_e {
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_1 = 1ULL << 0,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_2 = 1ULL << 1,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_4 = 1ULL << 2,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_8 = 1ULL << 3,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_16 = 1ULL << 4,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_32 = 1ULL << 5,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_64 = 1ULL << 6,
    GEOPM_FLAGS_SMALL_CPU_FREQ_100MHZ_128 = 1ULL << 7,
    GEOPM_FLAGS_BIG_CPU_NUM_1 = 1ULL << 8,
    GEOPM_FLAGS_BIG_CPU_NUM_2 = 1ULL << 9,
    GEOPM_FLAGS_BIG_CPU_NUM_4 = 1ULL << 10,
    GEOPM_FLAGS_BIG_CPU_NUM_8 = 1ULL << 11,
    GEOPM_FLAGS_BIG_CPU_NUM_16 = 1ULL << 12,
    GEOPM_FLAGS_BIG_CPU_NUM_32 = 1ULL << 13,
    GEOPM_FLAGS_BIG_CPU_NUM_64 = 1ULL << 14,
    GEOPM_FLAGS_BIG_CPU_NUM_128 = 1ULL << 15,
    GEOPM_FLAGS_SMALL_CPU_TOPOLOGY_COMPACT = 1ULL << 16,
    GEOPM_FLAGS_SMALL_CPU_TOPOLOGY_SCATTER = 1ULL << 17,
    GEOPM_FLAGS_TDP_PERCENT_1 = 1ULL << 18,
    GEOPM_FLAGS_TDP_PERCENT_2 = 1ULL << 19,
    GEOMP_FLAGS_TDP_PERCENT_4 = 1ULL << 20,
    GEOMP_FLAGS_TDP_PERCENT_8 = 1ULL << 21,
    GEOMP_FLAGS_TDP_PERCENT_16 = 1ULL << 22,
    GEOMP_FLAGS_TDP_PERCENT_32 = 1ULL << 23,
    GEOMP_FLAGS_TDP_PERCENT_64 = 1ULL << 24,
    GEOPM_FLAGS_GOAL_CPU_EFFICIENCY = 1ULL << 25,
    GEOPM_FLAGS_GOAL_NETWORK_EFFICIENCY = 1ULL << 26,
    GEOPM_FLAGS_GOAL_MEMORY_EFFICIENCY = 1ULL << 27,
};

/// @brief Enum encompassing geopm power management modes.
enum geopm_policy_mode_e {
    GEOPM_MODE_TDP_BALANCE_STATIC = 1,
    GEOPM_MODE_FREQ_UNIFORM_STATIC = 2,
    GEOPM_MODE_FREQ_HYBRID_STATIC = 3,
    GEOPM_MODE_PERF_BALANCE_DYNAMIC = 4,
    GEOPM_MODE_FREQ_UNIFORM_DYNAMIC = 5,
    GEOPM_MODE_FREQ_HYBRID_DYNAMIC = 6,
    GEOPM_MODE_SHUTDOWN = 255,
};

/// @brief Enum encompassing application region
/// characteristic hints.
enum geopm_policy_hint_e {
    GEOPM_POLICY_HINT_UNKNOWN = 0,
    GEOPM_POLICY_HINT_COMPUTE = 1,
    GEOPM_POLICY_HINT_MEMORY = 2,
    GEOPM_POLICY_HINT_NETWORK = 3,
};

/// @brief Enum encompassing application and
/// geopm runtime state.
enum geopm_status_e {
    GEOPM_STATUS_UNDEFINED = 0,
    GEOPM_STATUS_INITIALIZED = 1,
    GEOPM_STATUS_ACTIVE = 2,
    GEOPM_STATUS_REPORT = 3,
    GEOPM_STATUS_READY = 4,
    GEOPM_STATUS_SHUTDOWN = 5,
};

/// @brief MPI message structure for sending
/// power policies down the tree.
struct geopm_policy_message_s {
    /// @brief 64-bit unique application region identifier.
    uint64_t region_id;
    /// @brief Enum power management mode.
    int mode;
    /// @brief Power policy attribute bitmask
    unsigned long flags;
    /// @brief Number of samples to collect before sending
    /// a sample up the tree.
    int num_sample;
    /// @brief Power budget in Watts.
    double power_budget;
};

/// @brief Structure intended to be shared between
/// the resource manager and the geopm
/// runtime in order to convey job wide
/// power policy changes to the geopm runtime.
struct geopm_policy_shmem_s {
    /// @brief Enables the geopm runtime know when the 
    /// resource manager has initialized the
    /// power policy.
    int is_init;
    /// @brief Lock to ensure read/write consistency
    /// between the resource manager and the
    /// geopm runtime.
    pthread_mutex_t lock;
    /// @brief Holds the job power policy as given
    /// by the resource manager.
    struct geopm_policy_message_s policy;
};

/// @brief MPI message structure for sending
/// sample telemetry data up the tree.
struct geopm_sample_message_s {
    /// @brief Rank identifier.
    int rank;
    /// @brief 64-bit unique application region identifier.
    uint64_t region_id;
    /// @brief Elapsed runtime of an application region.
    double runtime;
    /// @brief Energy used during an application region.
    double energy;
    /// @brief Average frequency during an application region.
    double frequency;
};

/// @brief Structure used to hold single profiling
/// messages obtained from the application.
struct geopm_prof_message_s {
    /// @brief Rank identifier.
    int rank;
    /// @brief 64-bit unique application region identifier.
    uint64_t region_id;
    /// @brief Time stamp of when the sample was taken.
    struct geopm_time_s timestamp;
    /// @brief Progress of the rank within the current region.
    double progress;
};

/// @brief Structure intended to be shared between
/// the geopm runtime and the application in
/// order to convey status and control information.
struct geopm_ctl_message_s {
    /// @brief Status of the geopm runtime.
    volatile uint32_t ctl_status;
    /// @brief Status of the application.
    volatile uint32_t app_status;
    /// @brief Holds affinities of all application ranks
    /// on the local compute node.
    int cpu_rank[GEOPM_CONST_MAX_NUM_CPU];
};

extern const struct geopm_policy_message_s GEOPM_UNKNOWN_POLICY;
extern const struct geopm_sample_message_s GEOPM_INVALID_SAMPLE;

/// @brief Check if two policy messages are equal.
/// @param [in] a Pointer to a policy message.
/// @param [in] b Pointer to a policy message.
/// @return 1 if policies are equal, else 0
int geopm_is_policy_equal(const struct geopm_policy_message_s *a, const struct geopm_policy_message_s *b);

#ifdef __cplusplus
}
#endif
#endif