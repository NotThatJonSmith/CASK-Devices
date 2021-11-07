#include <Schedule.hpp>

namespace CASK {

void Schedule::Tick() {
    for (auto &child : children) {
        child->Tick();
    }
}

void Schedule::BeforeFirstTick() {
    for (auto &child : children) {
        child->BeforeFirstTick();
    }
}

void Schedule::AddTickable(Tickable *child) {
    children.push_back(child);
}

} // namespace CASK