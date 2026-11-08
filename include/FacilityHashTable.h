// Purpose: Provides facility lookup by normalized ID.
// Author: Olu
#ifndef FACILITY_HASH_TABLE_H
#define FACILITY_HASH_TABLE_H

#include "Facility.h"

#include <cstddef>
#include <string>
#include <vector>

class FacilityHashTable {
public:
    explicit FacilityHashTable(std::size_t capacity = 17);

    bool insert(const Facility& facility);
    Facility* find(const std::string& id);
    const Facility* find(const std::string& id) const;

    std::size_t bucketCount() const;
    std::size_t size() const;
    std::size_t collisionCount() const;

    static std::string normalizeId(const std::string& id);
    static bool isValidId(const std::string& id);

private:
    std::vector<std::vector<Facility> > buckets_;
    std::size_t size_;
    std::size_t collisionCount_;

    std::size_t bucketIndex(const std::string& normalizedId) const;
};

#endif // FACILITY_HASH_TABLE_H
