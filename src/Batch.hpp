#ifndef BATCH_HPP
#define BATCH_HPP

#include <string>
#include <fstream>
#include <vector>

namespace fme
{

enum CommandType
{
    cmdCOMMENT,
    cmdMD,
    cmdMF,
    cmdRM,
    cmdCP,
    cmdMV
};

class Command
{
public:
    void parse(const std::string& s);

    CommandType type;
    std::vector<std::string> args;
};

class Batch
{
public:
    Batch(const std::string& fileName);
    friend Batch& operator>>(Batch& bat, Command& cmd);
    operator bool() const;

protected:
    std::ifstream in;
};

} // namespace fme

#endif // BATCH_HPP
