#pragma once

#include <Tickable.hpp>

#include <vector>

namespace CASK {

class Schedule : public Tickable {

public:

    virtual void Tick() override;
    virtual void BeforeFirstTick() override;
    void AddTickable(Tickable *child);

private:

    std::vector<Tickable*> children;
};

} // namespace CASK
