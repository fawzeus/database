#ifndef PAGER_HPP
#define PAGER_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

// STATUS: placeholder — implement in Milestone 5.
// Responsibility: fixed-size page cache over a .db file, the layer between
// in-memory structures and bytes on disk.

constexpr std::size_t PAGE_SIZE = 4096;
constexpr std::size_t MAX_PAGES = 100;  // raise later

using Page = std::array<uint8_t, PAGE_SIZE>;

class Pager {
public:
    explicit Pager(const std::string& filename);
    ~Pager();  // flush dirty pages

    Page* getPage(uint32_t pageNum);
    void flushPage(uint32_t pageNum);
    uint32_t numPages() const;

private:
    std::fstream file_;
    uint32_t numPages_ = 0;
    std::vector<std::optional<Page>> cache_;  // lazy-loaded page cache
};

#endif /* PAGER_HPP */
