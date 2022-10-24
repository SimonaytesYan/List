# List 
Cash friendly implementation of a list in C.

## List structure

You should determine type of list element value like this:
```c++
typedef int ListElem_t;
```

and also implement printElem function to log elements

## Logs
Use
```c++
#define LOGS_TO_FILE
```
If you want to put logs to file with name
```c++
const char LOGS
```
If you want to put logs to console use
```c++
#define LOGS_TO_CONSOLE
```
You should use this defines in Logging.cpp 