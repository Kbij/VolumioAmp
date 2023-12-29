#include "Utilities.h"
#include <glog/logging.h>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <memory>
#include <iomanip>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include "pstream.h"
#pragma GCC diagnostic pop

std::string runShell(const std::string& shellCmd)
{
	VLOG(1) << "Cmd: " << shellCmd;
	std::string result = "";

#ifdef _WIN32
	std::array<char, 128> buffer;
	std::shared_ptr<FILE> pipe(_popen(shellCmd.c_str(), "r"), _pclose);
	if (!pipe)
	{
		LOG(ERROR) << "popen() failed !";
	}
	else
	{
		while (!feof(pipe.get()))
		{
			if (fgets(buffer.data(), 128, pipe.get()) != NULL)
			{
				result += buffer.data();
			}
		}
	}
#else
	// run a process and create a streambuf that reads its stdout and stderr
	// Capturing stderr does not work; neet to add 2>&1 to each command
	const redi::pstreams::pmode mode = redi::pstreams::pstderr | redi::pstreams::pstdout;

	redi::ipstream proc(shellCmd, mode);
	std::string line;
	std::stringstream stdout;
	// read child's stdout
	bool firstLineWritten = false;
	while (std::getline(proc.out(), line))
	{
		if (firstLineWritten)
		{
			stdout << "\n";
		}
		VLOG(30) << "Line output: " << line;
		stdout << line;
		firstLineWritten = true;
	}

	std::stringstream stderr;
	// read child's stderr
	firstLineWritten = false;
	while (std::getline(proc.err(), line))
	{
		if (firstLineWritten)
		{
			stderr << "\n";
		}

		VLOG(30) << "Line output: " << line;
		stderr << line;
		firstLineWritten = true;
	}

	if (stdout.str() != "")
	{
		result += stdout.str();
	}

	if (stderr.str() != "")
	{
		result += "Error: " + stderr.str();
	}

#endif

	VLOG(30) << "Result: \n" << result;
	return result;
}