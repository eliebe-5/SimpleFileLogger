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
Initiate logger, this need to be called.
```c++
Log::init();
```
Commit buffer to file, call, as an example, once every frame.
```c++
Log::commit_to_file();
```
Cleanup logger, this should be called for safety, this will also commit any uncommited data to the file.
```c++
Log::cleanup();
```
Write functions. Error and warning prepend [ERROR] and [WARNING] respectively to the log before the message.
```c++
Log::write("A simple string.");
Log::warn("A simple warning.");
Log::error("A simple error.");
Log::writef("A printf style %s.", "string");
Log::warnf("A printf style %s.", "warning");
Log::errorf("A printf style %s.", "error");
```
