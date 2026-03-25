#include "double_buffer.h"

void swap_buffers(char** front, char** back)
{
	char* tmp = *front;
	*front = *back;
	*back = tmp;
}