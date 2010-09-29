#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)

#define MAJOR_VERSION	0
#define MINOR_VERSION	5

#define VERSION_STRING	QUOTE( MAJOR_VERSION ) "." QUOTE( MINOR_VERSION )