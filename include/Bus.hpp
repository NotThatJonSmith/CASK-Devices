#pragma once

#include <IOTarget.hpp>
#include <vector>

namespace CASK {

class Bus final : public IOTarget {

public:

    virtual __uint32_t Read32(__uint32_t startAddress, __uint32_t size, char* dst) override;
    virtual __uint64_t Read64(__uint64_t startAddress, __uint64_t size, char* dst) override;
    virtual __uint128_t Read128(__uint128_t startAddress, __uint128_t size, char* dst) override;
    virtual __uint32_t Write32(__uint32_t startAddress, __uint32_t size, char* src) override;
    virtual __uint64_t Write64(__uint64_t startAddress, __uint64_t size, char* src) override;
    virtual __uint128_t Write128(__uint128_t startAddress, __uint128_t size, char* src) override;
    virtual __uint32_t Fetch32(__uint32_t startAddress, __uint32_t size, char* src) override;
    virtual __uint64_t Fetch64(__uint64_t startAddress, __uint64_t size, char* src) override;
    virtual __uint128_t Fetch128(__uint128_t startAddress, __uint128_t size, char* src) override;

    void AddIOTarget32(IOTarget *target, __uint32_t address, __uint32_t mask);
    void AddIOTarget64(IOTarget *target, __uint64_t address, __uint64_t mask);
    void AddIOTarget128(IOTarget *target, __uint128_t address, __uint128_t mask);

private:

    template <typename T>
    struct BusMapping {
        T first;
        T last;
        T deviceStart;
        IOTarget *target;
    };

    std::vector<BusMapping<__uint32_t>> mappings32;
    std::vector<BusMapping<__uint64_t>> mappings64;
    std::vector<BusMapping<__uint128_t>> mappings128;

    template<typename T>
    inline std::vector<BusMapping<T>>* AddressMapForWidth() {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return &mappings32;
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return &mappings64;
        } else {
            return &mappings128;
        }
    }

    // TODO handle transactions that stride multiple mappings
    template<typename T, CASK::AccessType accessType>
    inline T TransactInternal(T startAddress, T size, char* buf) {
        for (BusMapping<T> &candidate : *AddressMapForWidth<T>()) {
            if (startAddress >= candidate.first && startAddress + size - 1 <= candidate.last ) {
                T deviceAddress = startAddress - candidate.deviceStart;
                return candidate.target->template Transact<T, accessType>(deviceAddress, size, buf);
            }
        }
        return 0;
    }

    // TODO be clear about the meaning of size (it's actually size-1 right now for max-int problem)
    template<typename T>
    inline void AddIOTarget(IOTarget *dev, T address, T sizeMinusOne) {
        T first = address;
        T last = address + sizeMinusOne;
        std::vector<BusMapping<T>>* map = AddressMapForWidth<T>();

        for (typename std::vector<BusMapping<T>>::iterator other = map->begin(); other != map->end(); other++) {

            if (first > other->last) { // First is after the other range
                continue;
            } else if (first > other->first) { // First is inside the other range
                if (last > other->last) { // Last is after the other range
                    // Other item has its end clipped off by the new mapping
                    other->last = first - 1;
                } else { // Last is inside the other range
                    BusMapping<T> highMapChunk = { last + 1, other->last, other->deviceStart, other->target };
                    other->last = first - 1;
                    // TODO keep the bigger half on top
                    other = map->emplace(other, highMapChunk);
                    other++;
                }
            } else { // First is before the other range
                if (last > other->last) { // Last is after the other range
                    // Other item is completely engulfed. Remove it!
                    map->erase(other);
                    // TODO log a warning
                } else if (last > other->first) { // Last is inside the other range
                    // Other item has its start clipped off by the new mapping
                    other->first = last + 1;
                } else { // Last is before the other range
                    // No overlap
                    continue;
                }
            }
        }
        map->push_back( { first, last, first, dev } );
    }

};

} // namespace CASK
