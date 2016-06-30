#include <string>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <map>
#include <stack>
#include <cstddef>

#include <iostream>

#include "Batch.hpp"
#include "FileSystem.hpp"
#include "DirectoryIterator.hpp"


int execBatch(const std::string& batchFileName)
{
    int lineNo = 0;

    try
    {
	fme::FileSystem fs;

	fme::Batch bat(batchFileName);
	fme::Command cmd;    

	while (bat >> cmd) {
	    ++lineNo;
	    switch (cmd.type) {
		case fme::cmdCOMMENT:
		    break;

		case fme::cmdMD:
		    fs.makeDirectory(cmd.args[0]);
		    break;

		case fme::cmdMF:
		    fs.makeFile(cmd.args[0]);
		    break;

		case fme::cmdRM:
		    fs.remove(cmd.args[0]);
		    break;

		case fme::cmdCP:
		    fs.copy(cmd.args[0], cmd.args[1]);
		    break;

		case fme::cmdMV:
		    fs.move(cmd.args[0], cmd.args[1]);
		    break;

		default:
		    throw std::runtime_error("Bad command type");
	    }
	}

	for (fme::DirectoryIterator it(fs.getRoot()); it != fme::DirectoryIterator(); ++it)
	    std::cout <<
		std::string((*it).level, ' ') + std::string((*it).level, ' ') <<
		//(*it).name << " (" << (*it).item << ")" << std::endl;
		(*it).name << std::endl;


    }
    catch (std::exception& e)
    {
	std::stringstream ss;
	ss << lineNo;
	std::string lineInfo = (lineNo > 0 ? " (line: " + ss.str() + ")" : "");
	std::cerr << "Error: " << e.what() << lineInfo << std::endl;
	return 1;
    }

    return 0;
}


int main(int argc, char* argv[])
{
    if (argc != 2) {
	std::cout << "Usage: fme <batch file name>" << std::endl;
	return 0;
    }
    return execBatch(argv[1]);
}
