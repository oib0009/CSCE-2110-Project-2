// Purpose: Implements facility lookup by normalized ID.
// Author: Olu
#include "FacilityHashTable.h"

#include <cctype>

FacilityHashTable::FacilityHashTable(std::size_t capacity)
    : buckets_(capacity == 0 ? 1 : capacity), size_(0), collisionCount_(0) {}

std::string FacilityHashTable::normalizeId(const std::string& id) {
    const std::string whitespace = " \t\r\n";
    const std::string::size_type first = id.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }

    std::string result = id.substr(
        first, id.find_last_not_of(whitespace) - first + 1);
    for (std::string::size_type i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(result[i])));
    }
    return result;
}

bool FacilityHashTable::isValidId(const std::string& id) {
    if (id.size() != 4 || id[0] != 'F') {
        return false;
    }
    for (std::string::size_type i = 1; i < id.size(); ++i) {
        if (id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}

std::size_t FacilityHashTable::bucketIndex(
    const std::string& normalizedId) const {
    std::size_t index = 0;
    for (std::string::size_type i = 0; i < normalizedId.size(); ++i) {
        index = (index * 31 + static_cast<unsigned char>(normalizedId[i]))
            % buckets_.size();
    }
    return index;
}

bool FacilityHashTable::insert(const Facility& facility) {
    const std::string id = normalizeId(facility.id);
    if (!isValidId(id) || find(id) != 0) {
        return false;
    }

    std::vector<Facility>& bucket = buckets_[bucketIndex(id)];
    if (!bucket.empty()) {
        ++collisionCount_;
    }

    Facility stored = facility;
    stored.id = id;
    bucket.push_back(stored);
    ++size_;
    return true;
}

Facility* FacilityHashTable::find(const std::string& id) {
    const std::string normalizedId = normalizeId(id);
    if (!isValidId(normalizedId)) {
        return 0;
    }

    std::vector<Facility>& bucket = buckets_[bucketIndex(normalizedId)];
    for (std::vector<Facility>::iterator facility = bucket.begin();
         facility != bucket.end(); ++facility) {
        if (facility->id == normalizedId) {
            return &*facility;
        }
    }
    return 0;
}

const Facility* FacilityHashTable::find(const std::string& id) const {
    const std::string normalizedId = normalizeId(id);
    if (!isValidId(normalizedId)) {
        return 0;
    }

    const std::vector<Facility>& bucket = buckets_[bucketIndex(normalizedId)];
    for (std::vector<Facility>::const_iterator facility = bucket.begin();
         facility != bucket.end(); ++facility) {
        if (facility->id == normalizedId) {
            return &*facility;
        }
    }
    return 0;
}

std::size_t FacilityHashTable::bucketCount() const {
    return buckets_.size();
}

std::size_t FacilityHashTable::size() const {
    return size_;
}

std::size_t FacilityHashTable::collisionCount() const {
    return collisionCount_;
}
