/*
 * Copyright (c) 2015, 2016, Intel Corporation
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

#ifndef RAPLPLATFORM_HPP_INCLUDE
#define RAPLPLATFORM_HPP_INCLUDE

#include "Region.hpp"
#include "Policy.hpp"
#include "Platform.hpp"

namespace geopm
{
    /// @brief This class provides an implementation of a concrete platform
    /// supporting processors which use RAPL for power limiting. This
    /// includes Sandy Bridge E, Ivy Bridge E, and Haswell E processors.
    class RAPLPlatform : public Platform
    {
        public:
            /// @brief Default constructor.
            RAPLPlatform();
            /// @brief Default destructor
            virtual ~RAPLPlatform();
            virtual int control_domain(void);
            virtual void set_implementation(PlatformImp* platform_imp);
            virtual bool model_supported(int platform_id, const std::string &description) const;
            virtual size_t capacity(void);
            virtual void sample(std::vector<struct geopm_msr_message_s> &msr_values);
            virtual void enforce_policy(uint64_t region_id, Policy &policy) const;
            virtual void bound(double &upper_bound, double &lower_bound);
        protected:
            /// @brief structure to hold buffer indicies for platform signals.
            struct m_buffer_index_s {
                int package0_pkg_energy;
                int package1_pkg_energy;
                int package0_pp0_energy;
                int package1_pp0_energy;
                int package0_dram_energy;
                int package1_dram_energy;
                int inst_retired_any_base;
                int clk_unhalted_core_base;
                int clk_unhalted_ref_base;
                int llc_victims_base;
                int num_slot;
            };
            /// @brief Structure of buffer indices to store data into to
            /// avoid map look-ups.
            struct m_buffer_index_s m_buffer_index;
            /// @brief Vector of signal read operations.
            std::vector<struct geopm_signal_descriptor> m_batch_desc;
            /// @brief Number of CPUs on the platform.
            int m_num_cpu;
            /// @brief Number of packages on the platform.
            int m_num_package;
            /// @brief Number of tiles on the platform.
            int m_num_tile;
            /// @brief platform capability description string.
            const std::string m_description;
            const int M_HSX_ID;
            const int M_IVT_ID;
            const int M_SNB_ID;
            const int M_BDX_ID;
            const int M_KNL_ID;
    };
}

#endif
