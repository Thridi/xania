#include "AffectList.hpp"

#include "Logging.hpp"

#include <range/v3/algorithm/find.hpp>
#include <range/v3/iterator/operations.hpp>

AFFECT_DATA &AffectList::add(const AFFECT_DATA &aff) {
    auto new_aff = new AFFECT_DATA{aff};
    new_aff->next = first_;
    first_ = new_aff;
    return *new_aff;
}

AFFECT_DATA &AffectList::add_at_end(const AFFECT_DATA &aff) {
    AFFECT_DATA **p_last_next = &first_;
    for (auto *ent = first_; ent; ent = ent->next)
        p_last_next = &ent->next;
    *p_last_next = new AFFECT_DATA{aff};
    return **p_last_next;
}

void AffectList::remove(const AFFECT_DATA &aff) {
    if (&aff == first_) {
        first_ = aff.next;
    } else {
        AFFECT_DATA *prev;

        for (prev = first_; prev != nullptr; prev = prev->next) {
            if (prev->next == &aff) {
                prev->next = aff.next;
                break;
            }
        }

        if (prev == nullptr) {
            bug("Affect_remove: cannot find paf.");
            // TODO may leak
            return;
        }
    }
    delete &aff; // UGH
}

AFFECT_DATA *AffectList::find_by_skill(int skill_number) {
    if (auto it = ranges::find(*this, skill_number, [](auto &af) { return af.type; }); it != end())
        return &*it;
    return nullptr;
}

const AFFECT_DATA *AffectList::find_by_skill(int skill_number) const {
    if (auto it = ranges::find(*this, skill_number, [](auto &af) { return af.type; }); it != end())
        return &*it;
    return nullptr;
}

size_t AffectList::size() const noexcept { return ranges::distance(begin(), end()); }

void AffectList::clear() {
    while (first_)
        remove(*first_);
}
