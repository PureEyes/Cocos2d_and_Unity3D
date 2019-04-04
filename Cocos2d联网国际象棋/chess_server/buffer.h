#ifndef BUFFER_H_
#define BUFFER_H_

#include <iostream>

#define BUFFER_SIZE 1024

/*
Packet := header + body
header := length + '|';
length := [0-9]+
body   := text

Example:

01A
12Hello World!
*/

struct buffer {
	int size;
	char data[BUFFER_SIZE];
};

class Buffer
{
public:
	Buffer();
	~Buffer();
	void buffer_init();

	int buffer_add(const char *tmp, int len);

	int buffer_del(int len);

	char* encode_packet(const char *text);

	char* parse_packet();
public:
	struct buffer *buf;
};
#endif