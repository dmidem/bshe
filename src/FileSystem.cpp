#include <string>
#include <stdexcept>
#include <iterator>
#include <map>

#include "FileSystem.hpp"

namespace fme
{

const char* srcNotFoundMsg = "Source file/directory not found";
const char* dstDirNotFoundMsg = "Destination directory not found";
const char* dstExistsMsg = "Destination file/directory already exists";
const char* toItselfMsg = "Cannot copy/move file/directory to itself";

const char* notFoundMsg = "File/directory not found";
const char* dirNotFoundMsg = "Directory not found";
const char* existsMsg = "File/directory already exists";
const char* invalidNameMsg = "Invalid file/directory name";


std::string getPathTop(std::string& name)
{
    if ((name == "") || (name[0] != '/'))
        return "";
    size_t i = name.find('/', 1);
    if (i == std::string::npos)
        i = name.size();
    std::string result = name.substr(1, i - 1);
    name = name.substr(i, name.size());
    return result;
}

std::string getPathBottom(std::string& name)
{
    size_t i = name.find_last_of('/');
    if (i == std::string::npos)
	return "";
    std::string result = name.substr(i + 1, name.size() - i - 1);
    name = name.substr(0, i);
    return result;
}


/*
    FSItem
*/


FSItem::FSItem(FSItem* parent) : parent(parent) { }


/*
    File
*/


File::File(FSItem* parent) : FSItem(parent) { }


FSItem* File::clone(FSItem* newParent) const
{
    //return new File(*this, newParent);
    return new File(newParent);
}


FSItem* File::transfer(FSItem* newParent)
{
    parent = newParent;
    return this;
}


/*
    Directory
*/


Directory::Directory() : FSItem(NULL) { }


Directory::Directory(FSItem* parent) : FSItem(parent) { }


Directory::Directory(const Directory& other) : FSItem(other) { }


Directory::~Directory()
{
    for (Directory::iterator it = begin(); it != end(); ++it)
	delete it->second;
}


Directory& Directory::operator = (const Directory& other)
{
    return *this;
}


FSItem* Directory::clone(FSItem* newParent) const
{
    //Directory* dir = new Directory(*this, newParent);
    Directory* dir = new Directory(newParent);
    for (Directory::const_iterator it = begin(); it != end(); ++it)
	dir->insert(Directory::MapPair(it->first, it->second->clone(dir)));
    return dir;
}


FSItem* Directory::transfer(FSItem* newParent)
{
    parent = newParent;
    return this;
}


FSItem* Directory::getItem(const std::string& name)
{
    Directory::iterator cur = find(name);
    return (cur == end() ? NULL : cur->second);
}


bool Directory::addItem(const std::string& name, FSItem* item)
{
    if (getItem(name))
        return false;
    insert(Directory::MapPair(name, item));
    return true;
}


bool Directory::removeItem(const std::string& name, const bool keepObject)
{
    Directory::iterator cur = find(name);
    if (cur == end())
	return false;
    if (!keepObject)
	delete cur->second;
    erase(cur);
    return true;
}


FSItem* Directory::findItem(std::string fullName)
{
    if (fullName == "")
        return this;

    FSItem* item;
    Directory* dir;

    std::string name = getPathTop(fullName);
    if ((name == "") || ((item = getItem(name)) == NULL))
	return NULL;
    if (fullName == "")
	return item;
    if ((dir = dynamic_cast<Directory*>(item)) == NULL)
	return NULL;

    return dir->findItem(fullName);
}


bool Directory::within(const FSItem* parentItem)
{
    for (FSItem* p = parent; p != NULL; p = p->parent)
	if (p == parentItem)
	    return true;
    return false;
}


/*
    FileSystem
*/


void FileSystem::makeDirectory(const std::string& fullName)
{
    makeItem<Directory>(fullName);
}


void FileSystem::makeFile(const std::string& fullName)
{
    makeItem<File>(fullName);
}


void FileSystem::remove(std::string fullName)
{
    std::string name = getPathBottom(fullName);
    if (name == "")
        throw std::runtime_error(notFoundMsg);
    Directory* dir = dynamic_cast<Directory*>(root.findItem(fullName));
    if (dir == NULL)
        throw std::runtime_error(notFoundMsg);
    if (!dir->removeItem(name, false))
        throw std::runtime_error(notFoundMsg);
}


void FileSystem::copy(const std::string& srcFullName, const std::string& dstFullName)
{
    moveItem(srcFullName, dstFullName, true);
}


void FileSystem::move(const std::string& srcFullName, const std::string& dstFullName)
{
    moveItem(srcFullName, dstFullName, false);
}


bool FileSystem::exists(const std::string& fullName)
{
    return (root.findItem(fullName) != NULL);
}


Directory* FileSystem::getRoot()
{
    return &root;
}


Directory* FileSystem::findDirectory(const std::string& fullName)
{
    if (fullName == "")
        throw std::runtime_error(invalidNameMsg);
    Directory* dir = dynamic_cast<Directory*>(root.findItem(fullName));
    return dir;
}


File* FileSystem::findFile(const std::string& fullName)
{
    if (fullName == "")
        throw std::runtime_error(invalidNameMsg);
    File* file = dynamic_cast<File*>(root.findItem(fullName));
    if (file == NULL)
        throw std::runtime_error(notFoundMsg);
    return file;
}


template <class T> void FileSystem::makeItem(std::string fullName)
{
    std::string name = getPathBottom(fullName);
    if (name == "")
        throw std::runtime_error(invalidNameMsg);
    Directory* dir = dynamic_cast<Directory*>(root.findItem(fullName));
    if (dir == NULL)
        throw std::runtime_error(dirNotFoundMsg);
    if (!dir->addItem(name, new T(dir)))
        throw std::runtime_error(existsMsg);
}


void FileSystem::moveItem(std::string srcFullName, std::string dstFullName, const bool keepSrc)
{

    std::string srcName = getPathBottom(srcFullName);
    if (srcName == "")
        throw std::runtime_error(srcNotFoundMsg);
    Directory* srcDir = dynamic_cast<Directory*>(root.findItem(srcFullName));
    if (srcDir == NULL)
        throw std::runtime_error(srcNotFoundMsg);
    FSItem* srcItem = srcDir->getItem(srcName);
    if (srcItem == NULL)
        throw std::runtime_error(srcNotFoundMsg);

    Directory* dstDir;
    std::string dstName;
    if ((dstDir = dynamic_cast<Directory*>(root.findItem(dstFullName))) != NULL)
	dstName = srcName;
    else {
	if ((dstName = getPathBottom(dstFullName)) == "")
	    throw std::runtime_error(dstDirNotFoundMsg);
	if ((dstDir = dynamic_cast<Directory*>(root.findItem(dstFullName))) == NULL)
    	    throw std::runtime_error(dstDirNotFoundMsg);
    }

    if (dstDir->within(srcDir) || ((dstDir == srcDir) && (dstName == srcName)))
	throw std::runtime_error(toItselfMsg);

    if (keepSrc) {
	if (!dstDir->addItem(dstName, srcItem->clone(dstDir)))
    	    throw std::runtime_error(dstExistsMsg);
    } else {
	if (!dstDir->addItem(dstName, srcItem->transfer(dstDir)))
	    throw std::runtime_error(dstExistsMsg);
	if (!srcDir->removeItem(srcName, true))
	    throw std::runtime_error(srcNotFoundMsg);
    }
}

} // namespace fme
