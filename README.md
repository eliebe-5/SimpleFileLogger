# SimpleFileLogger
A simple single header lightweight file logger.

# HowTo
The logger needs to be initiated, with or without a config struct.
And after the program is finished the logger should be cleand up.

```c++
int main()
{
  Log::init();
  
  ...
  
  Log::cleanup();
}
```

# Config
The logger has a few options. If you wish, you can create a config and init with it.

```c++
Log::Config c;
Log::init(c);
```

The config struct contains options:
- bool timestamps: toggle timestamps in the output. Defaults to on.
- float time_limit: set ms that each commit may run for. Default is HUGE_VAL.
- char* path: path and filename. "output.txt".
- bool log_info: log info about the logger itself. Default to false.

# Functions
