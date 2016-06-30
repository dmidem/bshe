#include <string>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

#include "Batch.hpp"

namespace fme
{

const char* badArgCountMsg = "Bad number of arguments";
const char* badCmdNameMsg = "Bad command name";


struct CommandDef
{
    const CommandType type;
    const std::string name;
    const size_t narg;
};

CommandDef commandDefs[] = {
    { cmdMD, "md", 1 },
    { cmdMF, "mf", 1 },
    { cmdRM, "rm", 1 },
    { cmdCP, "cp", 2 },
    { cmdMV, "mv", 2 }
};

const size_t commandDefsNum = sizeof(commandDefs) / sizeof(commandDefs[0]);


void Command::parse(const std::string& line)
{
    std::istringstream iss(line);

    std::string name = "";
    iss >> name;

    args.clear();

    if ((name == "") || (name[0] == '#')) {
	type = cmdCOMMENT;
	return;
    }

    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(args));

    size_t i = 0;
    for (; i < commandDefsNum; ++i)
	if (name == commandDefs[i].name) {
	    if (args.size() != commandDefs[i].narg)
		throw std::runtime_error(badArgCountMsg);
	    type = commandDefs[i].type;
	    break;
	}
    if (i >= commandDefsNum)
	throw std::runtime_error(badCmdNameMsg);
}


Batch::Batch(const std::string& fileName) : in(fileName.c_str())
{
    if (!in)
        throw std::runtime_error(std::string(std::strerror(errno)));
}

Batch& operator>>(Batch& bat, Command& cmd)
{
    std::string line;
    if (std::getline(bat.in, line))
	cmd.parse(line);
    return bat;
}

Batch::operator bool() const
{
    return in;
}

} // namespace fme
