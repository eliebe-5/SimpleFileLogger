#pragma once
#include <cmath>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <chrono>
#include <queue>
#include <assert.h>

namespace Log {

	void write(char*);
	void writef(char*, ...);

	/**
	Struct used in initializing the Logger
	*/

	struct Config
	{
		//Add system time stamps too logged items
		bool timestamps = true;
		//Time allowed for each commit to run in ms
		float time_limit = HUGE_VALF;
		//Path and name
		char* path = (char*)"output.txt";
		//Log the loggers info
		bool log_info = false;
	};

	struct Logger
	{
		std::queue<char*> queue;
		Config config;
		bool init = false;
		FILE* fp;
	};

	static Logger logger;

	void init(Config c = Config())
	{
		logger.fp = fopen(c.path, "w+");

		if (logger.fp == NULL)
			logger.init = false;
		else
			logger.init = true;

		assert(logger.init);

		logger.config = c;
		if(logger.config.log_info)
			write("[[[LOG INIT]]]");
	}

	void commit_to_file()
	{
		if (logger.config.log_info)
			write("[[[LOG COMMIT]]]");

		long double timer = 0;

		for (int i = 0; i < logger.queue.size() && timer < logger.config.time_limit;)
		{
			auto s1 = std::chrono::high_resolution_clock::now();

			/** Commit */

			if (logger.config.timestamps)
			{
				auto now = std::chrono::system_clock::now();
				auto duration = now.time_since_epoch();
				auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
				auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
				auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
				fprintf(logger.fp, "[%i:%i:%i]", hours % 24, mins % 60, secs % 60);
			}

			fprintf(logger.fp, logger.queue.front());

			fprintf(logger.fp, "\n");

			//delete[] logger.queue.front();

			logger.queue.pop();

			/** Done Commiting */

			auto s2 = std::chrono::high_resolution_clock::now();
			timer += (long double)std::chrono::duration_cast<std::chrono::nanoseconds>(s2 - s1).count() * 0.000001;
		}

	}

	void cleanup()
	{
		if (logger.config.log_info)
			write("[[[LOG CLEANUP]]]");
		while (logger.queue.size() > 0)
			commit_to_file();
		fclose(logger.fp);
	}

	/**
		Log functions
	*/

	void write(char* s)
	{
		char* w = new char[strlen(s)];
		strcpy(w, s);
		logger.queue.push(w);
	}

	void warn(char *s)
	{
		const char* war = "[WARNING]";
		char* w = new char[strlen(s) + strlen(war) + 1];
		strcpy(w, war);
		write(strcat(w, s));
	}

	void error(char *s)
	{
		const char* war = "[ERROR]";
		char* w = new char[strlen(s) + strlen(war) + 1];
		strcpy(w, war);
		write(strcat(w, s));
	}

	char *convert(unsigned int num, int base)
	{
		static char rep[] = "0123456789ABCDEF";
		static char buffer[50];
		char *ptr;

		ptr = &buffer[49];
		*ptr = '\0';

		do
		{
			*--ptr = rep[num%base];
			num /= base;
		} while (num != 0);

		return(ptr);
	}

	void fdewrap(char* string, va_list args)
	{
		std::string res;
		char* it;

		unsigned int i;
		char* s;
		double f;
		char buffer[32];

		for (it = string; *it != '\0'; it++)
		{
			while (*it != '%' && it != '\0')
			{
				res += *it;
				it++;
			}

			if (it == '\0')
			{
				res += '\0';
				break;
			}
			it++;

			switch (*it)
			{
			case 'c': 
				i = va_arg(args, int);
				res += (char)i;
				break;
			case 'd':
			case 'i':
				i = va_arg(args, int);
				if (i < 0)
				{
					//i = -i;
					res += '-';
				}
				res += convert(i, 10);
				break;
			case 'f':
				f = va_arg(args, double);
				res += std::to_string(f);
				break;
			case 'o':
				i = va_arg(args, unsigned int);
				res += convert(i, 8);
				break;
			case 's':
				s = va_arg(args, char*);
				res += s;
				break;
			case 'x':
				i = va_arg(args, unsigned int);
				res += convert(i, 16);
				break;
			}
		}

		

		char* test = new char[res.size()]; 
		strcpy(test, res.c_str());
		logger.queue.push(test);

		va_end(args);

	}

	void writef(char* s, ...)
	{
		va_list ap;
		va_start(ap, s);

		fdewrap(s, ap);
	}

	void warnf(char *s, ...)
	{
		const char* war = "[WARNING]";
		char* w = new char[strlen(s) + strlen(war) + 1];
		strcpy(w, war);
		strcat(w, s);

		va_list ap;
		va_start(ap, s);

		fdewrap(w, ap);
		delete[] w;
	}

	void errorf(char *s, ...)
	{
		const char* war = "[ERROR]";
		char* w = new char[strlen(s) + strlen(war) + 1];
		strcpy(w, war);
		strcat(w, s);
	
		va_list ap;
		va_start(ap, s);
		
		fdewrap(w, ap);
		delete[] w;
	}
}
