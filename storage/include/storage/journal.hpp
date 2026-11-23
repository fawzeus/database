#ifndef JOURNAL_HPP
#define JOURNAL_HPP

#include <fstream>
#include <string>
#include <storage/pager.hpp>

// STATUS: placeholder — implement in Milestone 8.
// Responsibility: rollback journal. Copy each original page before changing
// it; deleting the journal is the commit point; on crash, restore from it.
class Journal {
public:
    explicit Journal(const std::string& dbPath);
    void saveOriginalPage(uint32_t pageNum, const Page& original);
    void commit();                // delete journal
    void rollback(Pager& pager);  // restore original pages
    bool exists() const;

private:
    std::string journalPath_;
    std::fstream file_;
};

#endif /* JOURNAL_HPP */
