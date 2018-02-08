#include <stdio.h>
#include "logger.h"

int main()
{
	Log::init();
	
	Log::write("I am just a simple string.");

	Log::writef("%c am a %s style string. Here is a number: %d and another: %f", 'I', "printf", 100, 12.34f);
	
	Log::commit_frame();
	
	Log::cleanup();

}
