#include <string>
#include <iterator>
#include <stack>

#include "DirectoryIterator.hpp"

namespace fme
{

DirectoryIterator::DirectoryIterator() : eof(true) { }


DirectoryIterator::DirectoryIterator(const Directory& dir)
{
    cur = dir.begin();
    end = dir.end();
    eof = (cur == end);
}


DirectoryIterator::DirectoryIterator(const Directory* dir)
{
    if (dir) {
        cur = dir->begin();
        end = dir->end();
    }
    eof = ((dir == NULL) || (cur == end));
}


DirectoryIterator& DirectoryIterator::operator ++ ()
{
    if (!eof)
        next();
    return *this;
}


DirectoryIterator DirectoryIterator::operator ++ (int)
{
    DirectoryIterator tmp(*this);
    operator++();
    return tmp;
}


bool DirectoryIterator::operator==(const DirectoryIterator& rhs)
{
    if (eof || rhs.eof)
        return eof == rhs.eof;
    return &cur->second == &rhs.cur->second;
}


bool DirectoryIterator::operator!=(const DirectoryIterator& rhs)
{
    return !operator==(rhs);
}


const DirectoryIterator::Result DirectoryIterator::operator*()
{
    if (eof)
        return *static_cast<const Result*>(NULL);
    return Result(cur->first, dumpStack.size(), cur->second);
}


const FSItem* DirectoryIterator::next()
{
    if (eof || (cur == end))
        return NULL;

    const Directory* dir = dynamic_cast<const Directory*>(cur->second);

    ++cur;

    if (dir && (!dir->empty())) {
	dumpStack.push(StackPair(cur, end));
	cur = dir->begin();
	end = dir->end();
	return cur->second;
    }

    while ((cur == end) && (!dumpStack.empty())) {
	StackPair top = dumpStack.top();
	cur = top.first;
	end = top.second;
	dumpStack.pop();
    }
    if ((cur == end) && dumpStack.empty()) {
	eof = (cur == end);
	return NULL;
    }

    return cur->second;
}

} // namespace fme
