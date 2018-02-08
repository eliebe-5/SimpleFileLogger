# SimpleFileLogger
A simple single header lightweight file logger.

#HowTo
The logger needs to be initiated, with or without a config struct.
And after the program is finished the logger should be cleand up.

```
int main()
{
  Log::init();
  
  ...
  
  Log::cleanup();
}
```
