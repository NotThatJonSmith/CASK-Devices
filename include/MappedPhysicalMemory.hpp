#pragma once

#include <cstring>
#include <type_traits>

#include <IOTarget.hpp>

namespace CASK {

class MappedPhysicalMemory final : public CASK::IOTarget {

public:

    MappedPhysicalMemory();
    virtual __uint32_t Read32(__uint32_t startAddress, __uint32_t size, char* dst) override;
    virtual __uint64_t Read64(__uint64_t startAddress, __uint64_t size, char* dst) override;
    virtual __uint128_t Read128(__uint128_t startAddress, __uint128_t size, char* dst) override;
    virtual __uint32_t Write32(__uint32_t startAddress, __uint32_t size, char* src) override;
    virtual __uint64_t Write64(__uint64_t startAddress, __uint64_t size, char* src) override;
    virtual __uint128_t Write128(__uint128_t startAddress, __uint128_t size, char* src) override;
    virtual __uint32_t Fetch32(__uint32_t startAddress, __uint32_t size, char* src) override;
    virtual __uint64_t Fetch64(__uint64_t startAddress, __uint64_t size, char* src) override;
    virtual __uint128_t Fetch128(__uint128_t startAddress, __uint128_t size, char* src) override;

private:

    char* memStartAddress;

    template <typename T, CASK::AccessType accessType>
    inline T TransactInternal(T startAddress, T size, char* buf) {

        if constexpr (accessType != AccessType::W) {
            memcpy(buf, memStartAddress+startAddress, size);
        } else {
            memcpy(memStartAddress+startAddress, buf, size);
        }

        return size;
    }
};

} // namespace CASK
