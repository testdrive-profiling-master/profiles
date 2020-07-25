//=======================================================================
// CloneX developments. Copyright 2013~2016. ALL RIGHTS RESERVED
// Title : Process filter
// Rev.  : 4/1/2016 Fri (clonextop@gmail.com)
//=======================================================================
#include <stdio.h>
#include <tchar.h>

// signal structure
typedef struct {
	union {
		unsigned int	all;
		struct {
			unsigned int	INTR	: 1;
			unsigned int	BUSY	: 1;
			unsigned int	nRST	: 1;
			unsigned int /*dummy*/	: 0;
		};
	};
} REG;

int _tmain(int argc, _TCHAR* argv[])
{
	REG	reg;

	while(!feof(stdin)) {
		char buf[32];
		buf[0] = 0;
		fscanf(stdin, "%s", buf);

		if(buf[0]) {
			sscanf(buf, "%x", &reg.all);

			if(reg.nRST) {
				if(reg.INTR)
					printf("?blue?INTERRUPT\n");
				else if(reg.BUSY)
					printf("?green?BUSY\n");
				else
					printf("IDLE\n");
			} else {
				printf("?dark red?RESET\n");
			}

			fflush(stdout);
		}
	}

	return 0;
}
