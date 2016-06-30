#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <map>

namespace fme
{

class FSItem
{
public:
    FSItem(FSItem* parent);
    virtual FSItem* clone(FSItem* newParent) const = 0;
    virtual FSItem* transfer(FSItem* newParent) = 0;

    FSItem* parent;
};


class File : public FSItem
{
public:
    File(FSItem* parent);
    FSItem* clone(FSItem* newParent) const;
    FSItem* transfer(FSItem* newParent);
};


class Directory : public std::map<std::string, FSItem*>, public FSItem
{
    typedef std::pair<std::string, FSItem*> MapPair;

public:
    Directory();
    Directory(FSItem* parent);
    Directory(const Directory& other);
    ~Directory();
    Directory& operator = (const Directory& other);

    FSItem* clone(FSItem* newParent) const;
    FSItem* transfer(FSItem* newParent);

    FSItem* getItem(const std::string& name);
    bool addItem(const std::string& name, FSItem* item);
    bool removeItem(const std::string& name, const bool keepObject);
    FSItem* findItem(std::string fullName);
    bool within(const FSItem* parentItem);
};


class FileSystem
{
public:
    void makeDirectory(const std::string& fullName);
    void makeFile(const std::string& fullName);
    void remove(std::string fullName);
    void copy(const std::string& srcFullName, const std::string& dstFullName);
    void move(const std::string& srcFullName, const std::string& dstFullName);
    bool exists(const std::string& fullName);

    Directory* getRoot();
    Directory* findDirectory(const std::string& fullName);
    File* findFile(const std::string& fullName);

protected:
    Directory root;

    template <class T> void makeItem(std::string name);
    void moveItem(std::string srcFullName, std::string dstFullName, const bool keepSrc);
};

} // namespace fme

#endif // FILESYSTEM_HPP
