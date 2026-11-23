#include <data/row.hpp>

#include <algorithm>
#include <sstream>
#include <type_traits>

// ---------- Value helpers ----------

const char* valueTypeName(const Value& value) {
    return std::visit([](const auto& v) -> const char* {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "NULL";
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return "INTEGER";
        } else if constexpr (std::is_same_v<T, double>) {
            return "REAL";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "TEXT";
        } else {
            return "BLOB";
        }
    }, value);
}

std::string valueToString(const Value& value) {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "NULL";
        } else if constexpr (std::is_same_v<T, std::vector<uint8_t>>) {
            return "[BLOB of size " + std::to_string(v.size()) + "]";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return v;
        } else {
            std::ostringstream out;
            out << v;
            return out.str();
        }
    }, value);
}

bool isNull(const Value& value) {
    return std::holds_alternative<std::monostate>(value);
}

// ---------- Row: modification ----------

errorId_t Row::addColumn(const std::string& columnName, const Value& value) {
    auto inserted = data_.emplace(columnName, value);
    if (!inserted.second) {
        return errorId_t::ERR_COLUMN_ALREADY_EXISTS;
    }
    columnOrder_.push_back(columnName);
    return errorId_t::ERR_STATUS_OK;
}

errorId_t Row::setValue(const std::string& columnName, const Value& value) {
    auto it = data_.find(columnName);
    if (it == data_.end()) {
        return errorId_t::ERR_COLUMN_NOT_FOUND;
    }
    it->second = value;
    return errorId_t::ERR_STATUS_OK;
}

errorId_t Row::removeColumn(const std::string& columnName) {
    auto it = data_.find(columnName);
    if (it == data_.end()) {
        return errorId_t::ERR_COLUMN_NOT_FOUND;
    }
    data_.erase(it);
    columnOrder_.erase(
        std::remove(columnOrder_.begin(), columnOrder_.end(), columnName),
        columnOrder_.end()
    );
    return errorId_t::ERR_STATUS_OK;
}

void Row::clear() {
    data_.clear();
    columnOrder_.clear();
}

// ---------- Row: lookup ----------

errorId_t Row::getValue(const std::string& columnName, Value& value) const {
    auto it = data_.find(columnName);
    if (it == data_.end()) {
        return errorId_t::ERR_COLUMN_NOT_FOUND;
    }
    value = it->second;
    return errorId_t::ERR_STATUS_OK;
}

errorId_t Row::get(size_t index, Value& value) const {
    if (index >= columnOrder_.size()) {
        return errorId_t::ERR_INDEX_OUT_OF_RANGE;
    }
    return getValue(columnOrder_[index], value);
}

bool Row::hasColumn(const std::string& columnName) const {
    return data_.find(columnName) != data_.end();
}

size_t Row::size() const {
    return data_.size();
}

errorId_t Row::getColumnCount(size_t& count) const {
    count = columnOrder_.size();
    return errorId_t::ERR_STATUS_OK;
}

errorId_t Row::getColumnName(size_t index, std::string& columnName) const {
    if (index >= columnOrder_.size()) {
        return errorId_t::ERR_INDEX_OUT_OF_RANGE;
    }
    columnName = columnOrder_[index];
    return errorId_t::ERR_STATUS_OK;
}

const std::vector<std::string>& Row::columnNames() const {
    return columnOrder_;
}

// ---------- Row: output ----------

void Row::print(std::ostream& os) const {
    for (const auto& columnName : columnOrder_) {
        auto it = data_.find(columnName);
        if (it == data_.end()) {
            continue;  // defensive: columnOrder_ and data_ should stay in sync
        }
        os << columnName << ": " << valueToString(it->second) << '\n';
    }
}

void Row::print() const {
    print(std::cout);
}

std::string Row::toString() const {
    std::ostringstream out;
    bool first = true;
    for (const auto& columnName : columnOrder_) {
        auto it = data_.find(columnName);
        if (it == data_.end()) {
            continue;
        }
        if (!first) {
            out << " | ";
        }
        out << columnName << ": " << valueToString(it->second);
        first = false;
    }
    return out.str();
}

// ---------- Row: comparison ----------

bool Row::operator==(const Row& other) const {
    return columnOrder_ == other.columnOrder_ && data_ == other.data_;
}

bool Row::operator!=(const Row& other) const {
    return !(*this == other);
}
