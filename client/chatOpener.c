#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include "chatOpener.h"
#include "chatDefs.h"

typedef enum
{
	FALSE,
	TRUE
} bool;

/* assist funcs */
static size_t deleteLine(char *_buffer, size_t _size, const char *_grpName, int *_pid);
CHAT_OPENER_ERR removePidFromFile(char *_fileName, char *_grpName, int *_pid);
/* end assist funcs */
CHAT_OPENER_ERR openChat(char *_grpIp, int _grpPort, char *_userName, char *_groupName, int _chatId)
{
	char geometryListener[13];
	char geometrySender[13];
	char listener[100];
	char sender[100];
	sprintf(geometryListener, "20x20+%d+250", _chatId * 100);
	sprintf(geometrySender, "20x5+%d+500", _chatId * 100);
	sprintf(listener, "gnome-terminal --hide-menubar --geometry %s -t %s -- ./listen.out %s %d %s", geometryListener, _groupName, _grpIp, _grpPort, _groupName);
	sprintf(sender, "gnome-terminal --hide-menubar --geometry %s -- ./send.out %s %d %s %s", geometrySender, _grpIp, _grpPort, _userName, _groupName);

	system(listener);
	system(sender);
	return 0;
}

CHAT_OPENER_ERR closeChat(char *_grpName)
{
	int listennerPid, senderPid;
	CHAT_OPENER_ERR check;
	if ( (removePidFromFile(SENDER_PID_FNAME, _grpName, &listennerPid)) != CHAT_OPENER_SUCCESS){ return check; } 
	if ( (removePidFromFile(SENDER_PID_FNAME, _grpName, &senderPid)) != CHAT_OPENER_SUCCESS){ return check; } 

	kill(listennerPid, SIGKILL);
	kill(senderPid, SIGKILL);
	
	return CHAT_OPENER_SUCCESS;
}


/* assist funcs */
CHAT_OPENER_ERR removePidFromFile(char *_fileName, char *_grpName, int *_pid)
{
	FILE *fp;
	struct stat st;
	char *_buffer;
	size_t newSize;

	if (stat(_fileName, &st) != -1) /* suc: 0, err: -1 */
	{
		fp = fopen(_fileName, "rb");
		if (fp != NULL)
		{
			_buffer = (char *)malloc(st.st_size);
			if (_buffer == NULL)
			{
				return FILE_READ_MALLOC_FAIL;
			}
			if (fread(_buffer, 1, st.st_size, fp) == st.st_size) /*st_size: Total size, in bytes  */
			{
				fclose(fp);

				size_t newSize = deleteLine(_buffer, st.st_size, _grpName, _pid);

				fp = fopen(_fileName, "wb");
				if (fp != NULL)
				{
					fwrite(_buffer, 1, newSize, fp);
					fclose(fp);
				}
				else
				{
					return FILE_CANT_BE_OPENED;
				}
			}
			free(_buffer);
		}
		else
		{
			perror(_fileName);
		}
	}
	else
	{
		return FILE_NOT_FOUND;
	}

	return CHAT_OPENER_SUCCESS;
}
static size_t deleteLine(char *_buffer, size_t _size, const char *_grpName, int *_pid)

{
	/* file format - goup name space pid */
	char *p = _buffer;
	char *q;
	bool done = FALSE;
	size_t len = strlen(_grpName);
	size_t newSize = 0;
	size_t restSize;
	size_t lineSize = 0;
	char *line, grpName;
	int pid;
	do
	{
		q = strchr(p, *_grpName);
		if (q != NULL)
		{
			if (strncmp(q, _grpName, len) == 0)
			{
				sscanf(q + len, "%d", _pid);
				lineSize = 1; /* include '\n' in line size */
				/* count number of chars in line */
				for (char *line = q; *line != '\n'; ++line)
				{
					++lineSize;
				}
				/* calculate length left after line by subtracting offsets */
				restSize = (size_t)((_buffer + _size) - (q + lineSize));

				/* move block with next line forward */
				memmove(q, q + lineSize, restSize);

				/* calculate new size */
				newSize = _size - lineSize;
				done = TRUE;
			}
			else
			{
				p = q + 1; /* continue search */
			}
		}
		else
		{
			puts("no such name");
			done = TRUE;
		}
	} while (!done);

	return newSize;
}
/* end assist funcs */