#pragma once


#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <sstream>

namespace z {

enum LogLevel {
	LDEBUG = 0,
	LINFO,
	LWARN,
	LERROR,
	LFATAL,
};

std::string const LogStr[] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL",
};

template<LogLevel LEVEL>
constexpr auto GetLogString() {
	if constexpr (LEVEL == LDEBUG) {
		return "DEBUG";
	}
	if constexpr (LEVEL == LINFO) {
		return "INFO";
	}
	if constexpr (LEVEL == LWARN) {
		return "WARN";
	}
	if constexpr (LEVEL == LERROR) {
		return "ERROR";
	}
	return "FATAL";
}

template<LogLevel LEVEL>
class Log {
public:

	template<typename... Args>
	Log(Args... args){
		mStream << GetLogString<LEVEL>() << " ";
		if constexpr(sizeof...(args) > 0)
			Print(args...);
	}

	template<typename T, typename... Args>
	void Print(const T& first, Args... args) {
		mStream << first << " ";
		if constexpr (sizeof...(args) > 0) {
			Print(args...);
		}
	}

#pragma warning(disable: 4722)
	~Log() {
		mStream << std::endl;
		std::cout << mStream.str();
		if constexpr (LEVEL == LFATAL) {
			exit(-1);
		}
	}
#pragma warning(default: 4722)

#pragma warning(disable: 4227)
	template<typename T>
	Log& const operator << (T&& t) {
		mStream << t;
		return *this;
	}
#pragma warning(default: 4227)

private:
	std::stringstream mStream;
};


}