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

#ifndef PLATFORMIMP_HPP_INCLUDE
#define PLATFORMIMP_HPP_INCLUDE

#ifndef NAME_MAX
#define NAME_MAX 1024
#endif

#include <sys/types.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <utility>
#include <string>

#include "PlatformTopology.hpp"

namespace geopm
{

    /* Platform IDs
    ((family << 8) + model)
    0x62A - Sandy Bridge
    0x62D - Sandy Bridge E
    0x63A - Ivy Bridge
    0x63E - Ivy Bridge E
    0x63C - Haswell
    0x645 - Haswell
    0x646 - Haswell
    0x63f - Haswell E
    */
    /// @brief This class provides an abstraction of specific functionality
    /// and attributes of different hardware implementations. It holds
    /// the platform topology of the underlying hardware as well as
    /// address offsets of Model Specific Registers.
    class PlatformImp
    {
        public:
            /// @brief default PlatformImp constructor
            PlatformImp();
            /// @brief default PlatformImp destructor
            virtual ~PlatformImp();

            ////////////////////////////////////////////////////////////////////
            //                     Topology Information                       //
            ////////////////////////////////////////////////////////////////////
            /// @brief Retrieve the number of packages present on
            /// the platform.
            /// @return number of packages.
            int package(void) const;
            /// @brief Retrieve the number of tiles present on
            /// the platform.
            /// @return number of tiles.
            int tile(void) const;
            /// @brief Retrieve the number of physical CPUs present
            /// on the platform.
            /// @return number of physical CPUs.
            int hw_cpu(void) const;
            /// @brief Retrieve the number of logical CPUs present
            /// on the platform.
            /// @return number of logical CPUs.
            int logical_cpu(void) const;
            /// @brief Retrieve the topology tree for the platform.
            /// @return PlatformTopology object holding the
            ///         current platform topology.
            PlatformTopology topology(void) const;

            ////////////////////////////////////////////////////////////////////
            //                     MSR read/write support                     //
            ////////////////////////////////////////////////////////////////////
            /// @brief Write a value to a Model Specific Register.
            /// @param [in] device_type enum device type can be
            ///        one of GEOPM_DOMAIN_PACKAGE, GEOPM_DOMAIN_CPU,
            ///        GEOPM_DOMAIN_TILE, or GEOPM_DOMAIN_BOARD_MEMORY.
            /// @param [in] device_index Numbered index of the specified type.
            /// @param [in] msr_name String name of the requested MSR.
            /// @param [in] value Value to write to the specified MSR.
            void write_msr(int device_type, int device_index, const std::string &msr_name, uint64_t value);
            /// @brief Write a value to a Model Specific Register.
            /// @param [in] device_type enum device type can be
            ///        one of GEOPM_DOMAIN_PACKAGE, GEOPM_DOMAIN_CPU,
            ///        GEOPM_DOMAIN_TILE, or GEOPM_DOMAIN_BOARD_MEMORY.
            /// @param [in] device_index Numbered index of the specified type.
            /// @param [in] msr_offset Address offset of the requested MSR.
            /// @param [in] value Value to write to the specified MSR.
            void write_msr(int device_type, int device_index, off_t msr_offset, uint64_t value);
            /// @brief Read a value from a Model Specific Register.
            /// @param [in] device_type enum device type can be
            ///        one of GEOPM_DOMAIN_PACKAGE, GEOPM_DOMAIN_CPU,
            ///        GEOPM_DOMAIN_TILE, or GEOPM_DOMAIN_BOARD_MEMORY.
            /// @param [in] device_index Numbered index of the specified type.
            /// @param [in] msr_name String name of the requested MSR.
            /// @return Value read from the specified MSR.
            uint64_t read_msr(int device_type, int device_index, const std::string &msr_name);
            /// @brief Read a value from a Model Specific Register.
            /// @param [in] device_type enum device type can be
            ///        one of GEOPM_DOMAIN_PACKAGE, GEOPM_DOMAIN_CPU,
            ///        GEOPM_DOMAIN_TILE, or GEOPM_DOMAIN_BOARD_MEMORY.
            /// @param [in] device_index Numbered index of the specified type.
            /// @param [in] msr_offset Address offset of the requested MSR.
            /// @return Value read from the specified MSR.
            uint64_t read_msr(int device_type, int device_index, off_t msr_offset);
            /// @brief Retrieve the address offset of a Model Specific Register.
            /// @param [in] msr_name String name of the requested MSR.
            /// @return Address offset of the requested MSR.
            off_t msr_offset(std::string msr_name);
            /// @brief Output a MSR whitelist for use with the Linux MSR driver.
            /// @param [in] file_desc File descriptor for output.
            void whitelist(FILE* file_desc);
            /// @brief Initialize the topology and MSR file descriptors.
            virtual void initialize(void);
            /// @brief Set the path to the MSR special file. In Linux this path
            /// is /dev/msr/cpu_num.
            /// @param [in] cpu_num Logical cpu number to set the path for.
            virtual void msr_path(int cpu_num);

            ////////////////////////////////////////////////////////////////////
            //              Platform dependent implementations                //
            ////////////////////////////////////////////////////////////////////
            /// @brief Does this PlatformImp support a specific platform.
            /// @param [in] platform_id Platform identifier specific to the
            ///        underlying hardware. On x86 platforms this can be obtained by
            ///        the cpuid instruction.
            /// @return true if this PlatformImp supports platform_id,
            ///         else false.
            virtual bool model_supported(int platform_id) = 0;
            /// @brief Retrieve the string name of the underlying platform.
            /// @return Underlying platform name.
            virtual std::string platform_name(void) = 0;
            /// @brief Reset MSRs to a default state.
            virtual void reset_msrs(void) = 0;

        protected:
            /// @brief Open a MSR special file.
            /// @param [in] cpu Number of logical cpu to open.
            void open_msr(int cpu);
            /// @brief Close a MSR special file.
            /// @param [in] cpu Number of logical cpu to close.
            void close_msr(int cpu);
            /// @brief Look up topology information to set member variables.
            virtual void parse_hw_topology(void);
            /// @brief Opens the per cpu special files, initializes the MSR offset
            /// map, initialize RAPL, CBO and fixed counter MSRs.
            virtual void initialize_msrs() = 0;
            /// @brief Holds the underlying hardware topology.
            PlatformTopology m_topology;
            /// @brief Holds the file descriptors for the per-cpu special files.
            std::vector<int> m_cpu_file_descs;
            /// @brief Map of MSR string name to address offset.
            std::map<std::string, std::pair<off_t, unsigned long> > m_msr_offset_map;
            /// @brief Number of logical CPUs.
            int m_logical_cpus;
            /// @brief Number of hardware CPUs.
            int m_hw_cpus;
            /// @brief Number of tiles.
            int m_tiles;
            /// @brief Number of packages.
            int m_packages;
            /// @brief File path to MSR special files.
            char m_msr_path[NAME_MAX];
    };
}

#endif