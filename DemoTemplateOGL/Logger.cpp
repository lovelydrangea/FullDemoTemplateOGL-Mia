#include <iostream>
#include <string>
#include <fstream>
#include "Logger.h"

using namespace LOGGER;

	LOG::LOG() {
		LOG("output.log");
	}
	LOG::LOG(std::string filename) {
		name = filename;
	}
	std::string LOG::getLogger() {
		return this->name;
	}
	void LOG::info(const std::string log) {
		info(log.c_str());
	}
	void LOG::info(const char *log) {
		std::ofstream f(this->name, std::ios::app);
		if (f.is_open()) {
			f << log << std::endl;
			f.close();
		}
	}

	LOG LOGS::getLOGGER() {
		return getLOGGER("output.log");
	}
	LOG LOGS::getLOGGER(std::string filename) {
		if (log.size() > 0) {
			for (LOG l : log) {
				if (l.getLogger().compare(filename) == 0)
					return l;
			}
		} else {
			LOG l(filename);
			log.push_back(l);
			return l;
		}
	}

	std::vector<LOG> LOGS::log;
