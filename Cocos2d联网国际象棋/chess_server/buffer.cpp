#include "stdafx.h"
#include "buffer.h"

Buffer::Buffer()
{
	buffer_init();
}

Buffer::~Buffer()
{
	delete buf;
	buf = nullptr;
}

void Buffer::buffer_init()
{
	buf = (buffer*)malloc(sizeof(buffer));
	buf->size = 0;
}

int Buffer::buffer_add(const char * tmp, int len)
{
	if (buf->size + len > BUFFER_SIZE) {
		return -1;
	}
	memcpy(buf->data + buf->size, tmp, len);
	buf->size += len;
	return len;
}

int Buffer::buffer_del(int len)
{
	int size = buf->size - len;
	if (size < 0) {
		return -1;
	}
	memcpy(buf->data, buf->data + len, size);
	buf->size -= len;
	return len;
}

char * Buffer::encode_packet(const char * text)
{
	int body_len = strlen(text);
	if (body_len > BUFFER_SIZE - 15) {
		return nullptr;
	}
	char header[20];
	int len = sizeof(header);
	int header_len = snprintf(header, len, "%d|", body_len);
	int packet_len = header_len + body_len;

	char *buff = (char *)malloc(packet_len + 1);
	snprintf(buff, packet_len + 1, "%s%s", header, text);
	return buff;
}

char * Buffer::parse_packet()
{
	if (buf->size == 0) {
		return nullptr;
	}
	int head_len;
	int body_len;

	{
		char *body = (char *)memchr(buf->data, '|', buf->size);
		if (body == nullptr) {
			//printf("[Partial Packet] header not ready, buffer %d\n", buf->size);
			return nullptr;
		}
		body++;
		head_len = body - buf->data;
	}

	{
		char header[20];
		memcpy(header, buf->data, head_len - 1); // no '|'
		header[head_len - 1] = '\0';
		body_len = atoi(header);
	}

	if (buf->size < head_len + body_len) {
		//printf("[Partial Packet] body not ready, buffer %d\n", buf->size);
		return nullptr;
	}

	char *body = (char*)malloc(body_len + 1);
	if (body_len > 0) {
		memcpy(body, buf->data + head_len, body_len);
	}
	body[body_len] = '\0';

	buffer_del(head_len + body_len);

	return body;
}