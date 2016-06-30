#ifndef DIRECTORYITERATOR_HPP
#define DIRECTORYITERATOR_HPP

#include <string>
#include <iterator>
#include <stack>

#include "FileSystem.hpp"

namespace fme
{

class DirectoryIterator : public std::iterator<std::output_iterator_tag, FSItem>
{
    typedef std::pair<Directory::const_iterator, Directory::const_iterator> StackPair;
    struct Result
    {
	Result(const std::string& name, const size_t level, const FSItem* item) : name(name), level(level), item(item) { }
	const std::string name;
	const size_t level;
	const FSItem* item;
    };

public:
    DirectoryIterator();
    DirectoryIterator(const Directory& dir);
    DirectoryIterator(const Directory* dir);

    DirectoryIterator& operator++();
    DirectoryIterator operator++(int);
    bool operator==(const DirectoryIterator& rhs);
    bool operator!=(const DirectoryIterator& rhs);
    const Result operator*();

protected:
    bool eof;
    Directory::const_iterator cur;
    Directory::const_iterator end;
    std::stack<StackPair> dumpStack;

    const FSItem* next();
};

} // namespace fme

#endif // DIRECTORYITERATOR_HPP
