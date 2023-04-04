#ifndef L2V_ANALYZER_H
#define L2V_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "v2l_debug.h"
#include "lxt_write.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#define SYMPRIME 500009

typedef struct Node*			nptr;
typedef struct HistEnt*		hptr;


typedef struct HistEnt {
	hptr					next;				// next transition in history
	TimeType				time;				// time of transition
	TimeType				previous_width;		// to avoid thrashing

	union {
		unsigned char		val;				// value: "0XU1"[val]
		char*				vector;				// pointer to a whole vector
	} v;
} HistEnt;

struct Node {
	char*					nname;				// ascii name of node
	HistEnt					head;				/* first entry in transition history */
	hptr					curr;				/* ptr. to current history entry */

	hptr*					harray;				// fill this in when we make a trace.. contains
	//  a ptr to an array of histents for bsearching
	int						numhist;			// number of elements in the harray
	char					notdead;			// indicates if this node gets a non-x value
	int						numtrans;			// number of transitions

	struct Node*			substnode;			// pointer to substitutions on buses
	struct Node*			substhead;			// pointer to substitution head (originator) on buses
};

struct symbol {
	struct symbol*			nextinaet;			// for aet node chaining
	struct HistEnt*			h;					// points to previous one

	struct symbol*			next;				// for hash chain
	char*					name;
	char					selected;			// for the clist object

	struct Node*			n;
};

struct slist {
	struct slist*			next;
	char*					str;
	int						len;
};

struct vcdsymbol {
	struct vcdsymbol*		next;
	struct lt_symbol*		ltsym;
	char*					name;
	char*					id;
	char*					value;
	struct queuedevent*		ev;					// only if vartype==V_EVENT
	struct Node**			narray;
	unsigned int			nid;
	int						msi, lsi;
	int						size;
	unsigned char			vartype;
};


struct queuedevent {
	struct queuedevent*		next;
	struct vcdsymbol*		sym;
	TimeType				last_event_time;    // make +1 == 0 if there's not an event there too
};


struct symbol* symfind(char*);
struct symbol* symadd(char*, int);
int __hash(char* s);
int sigcmp(char*, char*);
void quicksort(struct symbol**, int, int);

TimeType vcd_main(char* fname, char* lxname, int doclock);

#endif

