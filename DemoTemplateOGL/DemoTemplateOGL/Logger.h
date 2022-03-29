#include <iostream>
#include <vector>

#pragma once
#ifndef LOGGER_H
#define LOGGER_H

namespace LOGGER {
	class LOG {
	private:
		std::string name;
	public:
		LOG();
		LOG(std::string filename);
		std::string getLogger();
		void info(const std::string log);
		void info(const char *log);
	};

	class LOGS {
	private:
		static std::vector<LOG> log;
	public:
		static LOG getLOGGER();
		static LOG getLOGGER(std::string filename);
	};
}

#endif
