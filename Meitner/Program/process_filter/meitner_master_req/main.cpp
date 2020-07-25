#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	while(!feof(stdin)){
		char buf[256];
		buf[0] = 0;
		fscanf(stdin, "%s", buf);

		switch(buf[0]){
		case '0':
			printf("?dark red?BUSY\n");
			break;
		case '1':
			printf("READY\n");
			break;
		case '2':
			// request wait
			printf("?green?Wait...\n");
			break;
		case '3':
			{
				// request granted
				char ctemp;
				unsigned int	size, address;

				ctemp	= buf[3];buf[3]	= 0;
				sscanf(&buf[1], "%x", &size);
				buf[3]	= ctemp;
				sscanf(&buf[3], "%x", &address);

				printf("?blue?%d @ %04X:%04X\n", size+1, address>>16, address&0xFFFF);
			}
			break;
		default:
			printf("Invalid!\n");
			break;
		}
		fflush(stdout);
	}
	return 0;
}
