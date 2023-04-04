/*
 * Copyright (c) 2001-2012 Tony Bybell.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "config.h"
#include "lxt_write.h"
#include "vTestbench.h"
#include "v2l_analyzer.h"

// hash/symtable manipulation
static int lt_hash(const char* s)
{
	const char* p;
	char ch;
	unsigned int h = 0, h2 = 0, pos = 0, g;

	for(p = s; *p; p++) {
		ch = *p;
		h2 <<= 3;
		h2 -= ((unsigned int)ch + (pos++));		/* this handles stranded vectors quite well.. */
		h = (h << 4) + ch;

		if((g = h & 0xf0000000)) {
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}

	h ^= h2;						/* combine the two hashes */
	return(h % LT_SYMPRIME);
}


static struct lt_symbol* lt_symadd(struct lt_trace* lt, const char* name, int hv)
{
	struct lt_symbol* s;
	s = (struct lt_symbol*)calloc(1, sizeof(struct lt_symbol));
	strcpy(s->name = (char*)malloc((s->namlen = strlen(name)) + 1), name);
	s->next = lt->sym[hv];
	lt->sym[hv] = s;
	return(s);
}


static struct lt_symbol* lt_symfind(struct lt_trace* lt, const char* s)
{
	int hv;
	struct lt_symbol* temp;
	hv = lt_hash(s);

	if(!(temp = lt->sym[hv])) return(NULL); /* no hash entry, add here wanted to add */

	while(temp) {
		if(!strcmp(temp->name, s)) {
			return(temp); /* in table already */
		}

		if(!temp->next) break;

		temp = temp->next;
	}

	return(NULL); /* not found, add here if you want to add*/
}


// initialize the trace and get back an lt context
struct lt_trace* lt_init(void)
{
	struct lt_trace* lt = (struct lt_trace*)calloc(1, sizeof(struct lt_trace));
	lt->initial_value = -1;				/* if a user sets this it will have a different POSITIVE val */
	lt->timescale = -256;				/* will be in range of -128<=x<=127 if set */
	lt->mintime = ULLDescriptor(1);
	lt->maxtime = ULLDescriptor(0);
	return(lt);
}

// clock flushing..
static void lt_flushclock(struct lt_trace* lt, struct lt_symbol* s)
{
	unsigned int start_position = lt->position;
	int numtrans = s->clk_numtrans - LT_CLKPACK - 1;

	if(numtrans < 0) {
		/* it never got around to caching */
		fprintf(stderr, "Possible Problem with %s with %d?\n", s->name, s->clk_numtrans);
		return;
	}

	s->last_change = start_position;
	/* s->clk_prevval		CAN BE INFERRED! */
	/* s->clk_prevtrans		CAN BE INFERRED! */
	/* s->clk_delta 		CAN BE INFERRED! */
	/*printf("Clock finish for '%s' at %lld ~ %lld ending with '%c' for %d repeats over a switch delta of %d\n",
		   s->name, lt->timeval - (s->clk_delta * (s->clk_numtrans - LT_CLKPACK)), lt->timeval, s->clk_prevval, s->clk_numtrans - LT_CLKPACK, s->clk_delta);*/
	Testbench_PushClock(s, lt->timeval, s->clk_delta, s->clk_numtrans - LT_CLKPACK);
	s->clk_prevtrans = ULLDescriptor(~0);
	s->clk_numtrans = 0;
}

// multi-bits(int type?) clock flushing
static void lt_flushclock_m(struct lt_trace* lt, struct lt_symbol* s)
{
	unsigned int start_position = lt->position;
	int numtrans = s->clk_numtrans - LT_CLKPACK_M - 1;

	if(numtrans < 0) {
		/* it never got around to caching */
		fprintf(stderr, "Possible Problem with %s with %d?\n", s->name, s->clk_numtrans);
		return;
	}

	s->last_change = start_position;
	/* s->clk_prevval		CAN BE INFERRED! */
	/* s->clk_prevtrans		CAN BE INFERRED! */
	/* s->clk_delta 		CAN BE INFERRED! */
	/*printf("Clock finish for '%s' at %lld ~ %lld ending with '%08x' for %d repeats over a switch delta of %lld\n",
		   s->name, lt->timeval - (s->clk_delta * (s->clk_numtrans - LT_CLKPACK)), lt->timeval, s->clk_prevval, s->clk_numtrans - LT_CLKPACK_M, s->clk_delta);*/
	Testbench_PushClock(s, lt->timeval, s->clk_delta, s->clk_numtrans - LT_CLKPACK_M, true);
	s->clk_prevtrans = ULLDescriptor(~0);
	s->clk_numtrans = 0;
}

// close out the trace and fixate it
void lt_close(struct lt_trace* lt)
{
	lxttime_t lasttime = ULLDescriptor(0);
	int lastposition = 0;
	char is64 = 0;

	if(lt) {
		struct lt_symbol* s = lt->symchain;

		if(lt->clock_compress)
			while(s) {
				if(s->clk_prevtrans != ULLDescriptor(~0)) {
					int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;

					if(len > 1) {
						if(s->clk_numtrans > LT_CLKPACK_M) lt_flushclock_m(lt, s);
					} else {
						if(s->clk_numtrans > LT_CLKPACK) lt_flushclock(lt, s);
					}
				}

				s = s->symchain;
			}

		vTestbenchVar::Finalize();
		free(lt->timebuff);
		lt->timebuff = NULL;

		if(lt->timehead) {
			struct lt_timetrail* t = lt->timehead;
			struct lt_timetrail* t2;
			is64 = (lt->maxtime > ULLDescriptor(0xFFFFFFFF));

			while(t) {
				lastposition = t->position;
				t = t->next;
			}

			t = lt->timehead;

			if(is64) {
				while(t) {
					lxttime_t delta = t->timeval - lasttime;
					lasttime = t->timeval;
					t2 = t->next;
					free(t);
					t = t2;
				}
			} else {
				while(t) {
					lasttime = t->timeval;
					t2 = t->next;
					free(t);
					t = t2;
				}

				lt->timehead = lt->timecurr = NULL;
			}
		}

		if(lt->initial_value >= 0) {
			lt->initial_value_offset = lt->position;
		}

		if((lt->timescale > -129) && (lt->timescale < 128)) {
			lt->timescale_offset = lt->position;
		}

		if(lt->symchain) {
			struct lt_symbol* sc = lt->symchain;
			struct lt_symbol* s2;

			while(sc) {
				free(sc->name);
				s2 = sc->symchain;
				free(sc);
				sc = s2;
			}
		}

		free(lt->sorted_facs);
		free(lt);
	}
}


// add a trace (if it doesn't exist already)
struct lt_symbol* lt_symbol_add(struct lt_trace* lt, struct vcdsymbol* v, const char* name, unsigned int rows, int msb, int lsb, int flags)
{
	struct lt_symbol* s;
	int len;
	int flagcnt;

	if((!lt) || (lt->sorted_facs)) return(NULL);

	flagcnt = ((flags & LT_SYM_F_INTEGER) != 0) + ((flags & LT_SYM_F_DOUBLE) != 0) + ((flags & LT_SYM_F_STRING) != 0);

	if((flagcnt > 1) || (!lt) || (!name) || (lt_symfind(lt, name))) return (NULL);

	if(flags & LT_SYM_F_DOUBLE) lt->double_used = 1;

	s = lt_symadd(lt, name, lt_hash(name));
	s->rows = rows;
	s->flags = flags & (~LT_SYM_F_ALIAS);	/* aliasing makes no sense here.. */
	s->vartype	= v->vartype;

	if(!flagcnt) {
		s->msb = msb;
		s->lsb = lsb;
		s->len = (msb < lsb) ? (lsb - msb + 1) : (msb - lsb + 1);

		if((s->len == 1) && (s->rows == 0)) s->clk_prevtrans = ULLDescriptor(~0);
	}

	s->symchain = lt->symchain;
	lt->symchain = s;
	lt->numfacs++;

	if((len = strlen(name)) > lt->longestname) lt->longestname = len;

	lt->numfacbytes += (len + 1);
	AddTestbenchSignal(s);
	return(s);
}

// add an alias trace (if it doesn't exist already and orig is found)
struct lt_symbol* lt_symbol_alias(struct lt_trace* lt, struct vcdsymbol* v, const char* existing_name, const char* alias, int msb, int lsb)
{
	struct lt_symbol* s, *sa;
	int len;
	int bitlen;
	int flagcnt;

	if((!lt) || (!existing_name) || (!alias) || (!(s = lt_symfind(lt, existing_name))) || (lt_symfind(lt, alias))) return (NULL);

	if(lt->sorted_facs) return(NULL);

	while(s->aliased_to) {	/* find root alias */
		s = s->aliased_to;
	}

	flagcnt = ((s->flags & LT_SYM_F_INTEGER) != 0) + ((s->flags & LT_SYM_F_DOUBLE) != 0) + ((s->flags & LT_SYM_F_STRING) != 0);
	bitlen = (msb < lsb) ? (lsb - msb + 1) : (msb - lsb + 1);

	if((!flagcnt) && (bitlen != s->len)) return(NULL);

	sa = lt_symadd(lt, alias, lt_hash(alias));
	sa->flags = LT_SYM_F_ALIAS;	/* only point this can get set */
	sa->aliased_to = s;
	sa->vartype	= v->vartype;

	if(!flagcnt) {
		sa->msb = msb;
		sa->lsb = lsb;
		sa->len = bitlen;
	}

	sa->symchain = lt->symchain;
	lt->symchain = sa;
	lt->numfacs++;

	if((len = strlen(alias)) > lt->longestname) lt->longestname = len;

	lt->numfacbytes += (len + 1);
	AddTestbenchSignal(sa);
	return(sa);
}


// set current time
int lt_inc_time_by_delta(struct lt_trace* lt, unsigned int timeval)
{
	return(lt_set_time64(lt, lt->maxtime + (lxttime_t)timeval));
}

int lt_set_time(struct lt_trace* lt, unsigned int timeval)
{
	return(lt_set_time64(lt, (lxttime_t)timeval));
}

int lt_set_time64(struct lt_trace* lt, lxttime_t timeval)
{
	int rc = 0;

	if(lt) {
		struct lt_timetrail* trl = (struct lt_timetrail*)calloc(1, sizeof(struct lt_timetrail));

		if(trl) {
			trl->timeval = timeval;
			trl->position = lt->position;

			if((lt->timecurr) || (lt->timebuff)) {
				if(((timeval > lt->mintime) && (timeval > lt->maxtime)) || ((lt->mintime == ULLDescriptor(1)) && (lt->maxtime == ULLDescriptor(0)))) {
					lt->maxtime = timeval;
				} else {
					free(trl);
					goto bail;
				}
			} else {
				lt->mintime = lt->maxtime = timeval;
			}

			free(lt->timebuff);
			lt->timebuff = trl;
			lt->timeval = timeval;
			rc = 1;
		}
	}

bail:
	return(rc);
}


// sets trace timescale as 10**x seconds
void lt_set_timescale(struct lt_trace* lt, int timescale)
{
	if(lt) {
		lt->timescale = timescale;
	}
}


// sets clock compression heuristic
void lt_set_clock_compress(struct lt_trace* lt)
{
	if(lt) {
		lt->clock_compress = 1;
	}
}

// sets trace initial value
void lt_set_initial_value(struct lt_trace* lt, char value)
{
	if(lt) {
		int tag;

		switch(value) {
		case '0':
			tag = 0;
			break;

		case '1':
			tag = 1;
			break;

		case 'Z':
		case 'z':
			tag = 2;
			break;

		case 'X':
		case 'x':
			tag = 3;
			break;

		case 'H':
		case 'h':
			tag = 4;
			break;

		case 'U':
		case 'u':
			tag = 5;
			break;

		case 'W':
		case 'w':
			tag = 6;
			break;

		case 'L':
		case 'l':
			tag = 0x7;
			break;

		case '-':
			tag = 0x8;
			break;

		default:
			tag = -1;
			break;
		}

		lt->initial_value = tag;
	}
}

// emission for trace values..
static int lt_optimask[] = {
	0x00000000,

	0x00000001,
	0x00000003,
	0x00000007,
	0x0000000f,

	0x0000001f,
	0x0000003f,
	0x0000007f,
	0x000000ff,

	0x000001ff,
	0x000003ff,
	0x000007ff,
	0x00000fff,

	0x00001fff,
	0x00003fff,
	0x00007fff,
	0x0000ffff,

	0x0001ffff,
	0x0003ffff,
	0x0007ffff,
	0x000fffff,

	0x001fffff,
	0x003fffff,
	0x007fffff,
	0x00ffffff,

	0x01ffffff,
	0x03ffffff,
	0x07ffffff,
	0x0fffffff,

	0x1fffffff,
	0x3fffffff,
	0x7fffffff,
	0xffffffff
};

int lt_emit_value_int(struct lt_trace* lt, struct lt_symbol* s, unsigned int row, int value)
{
	int rc = 0;

	if((!lt) || (!s)) return(rc);

	if(!lt->emitted) lt->emitted = 1;

	while(s->aliased_to) {	/* find root alias if exists */
		s = s->aliased_to;
	}

	if(!(s->flags & (LT_SYM_F_DOUBLE | LT_SYM_F_STRING))) {
		int numbytes;				/* number of bytes to store value minus one */
		unsigned int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;
		//printf("emit int : %s : %d\n", s->name, value);

		if((lt->clock_compress) && (s->rows == 0)) {
			if((len > 1) && (len <= 32)) {
				int ivalue = value;
				int delta1, delta2;
				s->clk_mask <<= 1;
				s->clk_mask |= 1;

				if(((s->clk_mask & 0x1f) == 0x1f) &&
				   ((delta1 = (ivalue         - s->clk_prevval1) & lt_optimask[s->len]) == ((s->clk_prevval1 - s->clk_prevval3) & lt_optimask[s->len])) &&
				   ((delta2 = (s->clk_prevval - s->clk_prevval2) & lt_optimask[s->len]) == ((s->clk_prevval2 - s->clk_prevval4) & lt_optimask[s->len])) &&
				   ((delta1 == delta2) || ((!delta1) && (!delta2)))
				  ) {
					if(s->clk_prevtrans == ULLDescriptor(~0)) {
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans = 0;
					} else if(s->clk_numtrans == 0) {
						s->clk_delta = lt->timeval - s->clk_prevtrans;
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans++;
					} else {
						if(s->clk_delta == (lt->timeval - s->clk_prevtrans)) {
							s->clk_numtrans++;
							s->clk_prevtrans = lt->timeval;

							if(s->clk_numtrans > LT_CLKPACK_M) {
								s->clk_prevval4 = s->clk_prevval3;
								s->clk_prevval3 = s->clk_prevval2;
								s->clk_prevval2 = s->clk_prevval1;
								s->clk_prevval1 = s->clk_prevval;
								s->clk_prevval  = ivalue;
								/* printf("Clock value '%08x' for '%s' at %lld (#%d)\n", ivalue, s->name, lt->timeval, s->clk_numtrans); */
								return(1);
							}
						} else {
							if(s->clk_numtrans > LT_CLKPACK_M) {
								lt_flushclock_m(lt, s);
								/* flush clock then continue below! */
							} else {
								s->clk_prevtrans = ULLDescriptor(~0);
							}
						}
					}
				} else {
					if(s->clk_numtrans > LT_CLKPACK_M) {
						lt_flushclock_m(lt, s);
						/* flush clock then continue below! */
					} else {
						s->clk_prevtrans = ULLDescriptor(~0);
					}
				}

				s->clk_prevval4 = s->clk_prevval3;
				s->clk_prevval3 = s->clk_prevval2;
				s->clk_prevval2 = s->clk_prevval1;
				s->clk_prevval1 = s->clk_prevval;
				s->clk_prevval  = ivalue;
			} else if(len == 1) { /* possible clock handling */
				int ivalue = value & 1;

				if(((s->clk_prevval == '1') && (ivalue == 0)) || ((s->clk_prevval == '0') && (ivalue == 1))) {
					if(s->clk_prevtrans == ULLDescriptor(~0)) {
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans = 0;
					} else if(s->clk_numtrans == 0) {
						s->clk_delta = lt->timeval - s->clk_prevtrans;
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans++;
					} else {
						if(s->clk_delta == (lt->timeval - s->clk_prevtrans)) {
							s->clk_numtrans++;
							s->clk_prevtrans = lt->timeval;

							if(s->clk_numtrans > LT_CLKPACK) {
								s->clk_prevval = ivalue + '0';
								/* printf("Clock value '%d' for '%s' at %d (#%d)\n", ivalue, s->name, lt->timeval, s->clk_numtrans); */
								return(1);
							}
						} else {
							if(s->clk_numtrans > LT_CLKPACK) {
								lt_flushclock(lt, s);
								/* flush clock then continue below! */
							} else {
								s->clk_prevtrans = ULLDescriptor(~0);
							}
						}
					}
				} else {
					if(s->clk_numtrans > LT_CLKPACK) {
						lt_flushclock(lt, s);
						/* flush clock then continue below! */
					} else {
						s->clk_prevtrans = ULLDescriptor(~0);
					}
				}

				s->clk_prevval = ivalue + '0';
			}
		}

		/* normal trace handling */
		if(len <= 32) {
			s->last_change = lt->position;
		}

		{
			char sValue[256];
			sprintf(sValue, "%d", value);
			Testbench_PushValue(s, lt->timeval, sValue);
		}

		if(lt->timebuff) {
			lt->timechangecount++;

			if(lt->timecurr) {
				lt->timecurr->next = lt->timebuff;
				lt->timecurr = lt->timebuff;
			} else {
				lt->timehead = lt->timecurr = lt->timebuff;
			}

			lt->timebuff = NULL;
		}
	}

	return(rc);
}


int lt_emit_value_double(struct lt_trace* lt, struct lt_symbol* s, unsigned int row, double value)
{
	int rc = 0;
	int start_position;
	int tag;

	if((!lt) || (!s)) return(rc);

	if(!lt->emitted) lt->emitted = 1;

	while(s->aliased_to) {	/* find root alias if exists */
		s = s->aliased_to;
	}

	if((s->flags)&LT_SYM_F_DOUBLE) {
		if(!lt->numfacs_bytes) {
			start_position = lt->position;
			s->last_change = start_position;
		}

		rc = 1;
		{
			char sValue[256];
			sprintf(sValue, "%f", value);
			Testbench_PushValue(s, lt->timeval, sValue);
		}

		if(lt->timebuff) {
			lt->timechangecount++;

			if(lt->timecurr) {
				lt->timecurr->next = lt->timebuff;
				lt->timecurr = lt->timebuff;
			} else {
				lt->timehead = lt->timecurr = lt->timebuff;
			}

			lt->timebuff = NULL;
		}
	}

	return(rc);
}


int lt_emit_value_string(struct lt_trace* lt, struct lt_symbol* s, unsigned int row, char* value)
{
	int rc = 0;

	if((!lt) || (!s) || (!value)) return(rc);

	if(!lt->emitted) lt->emitted = 1;

	while(s->aliased_to) {	/* find root alias if exists */
		s = s->aliased_to;
	}

	if((s->flags)&LT_SYM_F_STRING) {
		if(!lt->numfacs_bytes) {
			s->last_change = lt->position;
		}

		rc = 1;
		Testbench_PushValue(s, lt->timeval, value);

		if(lt->timebuff) {
			lt->timechangecount++;

			if(lt->timecurr) {
				lt->timecurr->next = lt->timebuff;
				lt->timecurr = lt->timebuff;
			} else {
				lt->timehead = lt->timecurr = lt->timebuff;
			}

			lt->timebuff = NULL;
		}
	}

	return(rc);
}


int lt_emit_value_bit_string(struct lt_trace* lt, struct lt_symbol* s, unsigned int row, char* value)
{
	int rc = 0;
	int start_position;
	int tag, tagadd;

	if((!lt) || (!s) || (!value) || (!*value)) return(rc);

	if(!lt->emitted) lt->emitted = 1;

	while(s->aliased_to) {	/* find root alias if exists */
		s = s->aliased_to;
	}

	if(!(s->flags & (LT_SYM_F_DOUBLE | LT_SYM_F_STRING))) {
		int numbytes;				/* number of bytes to store value minus one */
		char* pnt;
		int mvl = 0;
		char ch;
		char prevch;
		unsigned int len = ((s->flags)&LT_SYM_F_INTEGER) ? 32 : s->len;

		if((lt->clock_compress) && (s->rows == 0)) {
			if((len > 1) && (len <= 32)) {
				int legal = 0;
				int ivalue = 0;
				unsigned int i;
				char* pntv = value;
				int delta1, delta2;

				for(i = 0; i < len; i++) {
					if((*pntv != '0') && (*pntv != '1')) {
						if((!*pntv) && (i > 0)) {
							pntv--;
						} else {
							legal = 0;
							break;
						}
					}

					ivalue = (((unsigned int)ivalue) << 1);
					ivalue |= (*pntv & 1);
					legal = 1;
					pntv++;
				}

				s->clk_mask <<= 1;
				s->clk_mask |= legal;

				if(((s->clk_mask & 0x1f) == 0x1f) &&
				   ((delta1 = (ivalue         - s->clk_prevval1) & lt_optimask[s->len]) == ((s->clk_prevval1 - s->clk_prevval3) & lt_optimask[s->len])) &&
				   ((delta2 = (s->clk_prevval - s->clk_prevval2) & lt_optimask[s->len]) == ((s->clk_prevval2 - s->clk_prevval4) & lt_optimask[s->len])) &&
				   ((delta1 == delta2) || ((!delta1) && (!delta2)))
				  ) {
					if(s->clk_prevtrans == ULLDescriptor(~0)) {
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans = 0;
					} else if(s->clk_numtrans == 0) {
						s->clk_delta = lt->timeval - s->clk_prevtrans;
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans++;
					} else {
						if(s->clk_delta == (lt->timeval - s->clk_prevtrans)) {
							s->clk_numtrans++;
							s->clk_prevtrans = lt->timeval;

							if(s->clk_numtrans > LT_CLKPACK_M) {
								s->clk_prevval4 = s->clk_prevval3;
								s->clk_prevval3 = s->clk_prevval2;
								s->clk_prevval2 = s->clk_prevval1;
								s->clk_prevval1 = s->clk_prevval;
								s->clk_prevval  = ivalue;
								/* printf("Clock value '%08x' for '%s' [len=%d] at %lld (#%d)\n",
									ivalue, s->name, len, lt->timeval, s->clk_numtrans); */
								return(1);
							}
						} else {
							if(s->clk_numtrans > LT_CLKPACK_M) {
								lt_flushclock_m(lt, s);
								/* flush clock then continue below! */
							} else {
								s->clk_prevtrans = ULLDescriptor(~0);
							}
						}
					}
				} else {
					if(s->clk_numtrans > LT_CLKPACK_M) {
						lt_flushclock_m(lt, s);
						/* flush clock then continue below! */
					} else {
						s->clk_prevtrans = ULLDescriptor(~0);
					}
				}

				s->clk_prevval4 = s->clk_prevval3;
				s->clk_prevval3 = s->clk_prevval2;
				s->clk_prevval2 = s->clk_prevval1;
				s->clk_prevval1 = s->clk_prevval;
				s->clk_prevval  = ivalue;
			} else if(len == 1) { /* possible clock handling */
				if(((s->clk_prevval == '1') && (value[0] == '0')) || ((s->clk_prevval == '0') && (value[0] == '1'))) {
					if(s->clk_prevtrans == ULLDescriptor(~0)) {
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans = 0;
					} else if(s->clk_numtrans == 0) {
						s->clk_delta = lt->timeval - s->clk_prevtrans;
						s->clk_prevtrans = lt->timeval;
						s->clk_numtrans++;
					} else {
						if(s->clk_delta == (lt->timeval - s->clk_prevtrans)) {
							s->clk_numtrans++;
							s->clk_prevtrans = lt->timeval;

							if(s->clk_numtrans > LT_CLKPACK) {
								s->clk_prevval = value[0];
								/* printf("Clock value '%c' for '%s' at %lld (#%d)\n", value[0], s->name, lt->timeval, s->clk_numtrans); */
								return(1);
							}
						} else {
							if(s->clk_numtrans > LT_CLKPACK) {
								lt_flushclock(lt, s);
								/* flush clock then continue below! */
							} else {
								s->clk_prevtrans = ULLDescriptor(~0);
							}
						}
					}
				} else {
					if(s->clk_numtrans > LT_CLKPACK) {
						lt_flushclock(lt, s);
						/* flush clock then continue below! */
					} else {
						s->clk_prevtrans = ULLDescriptor(~0);
					}
				}

				s->clk_prevval = value[0];
			}
		}

		/* normal trace handling */
		pnt = value;
		prevch = *pnt;

		while((ch = *(pnt++))) {
			switch(ch) {
			case '0':
			case '1':
				mvl |= LT_MVL_2;
				break;

			case 'Z':
			case 'z':
			case 'X':
			case 'x':
				mvl |= LT_MVL_4;
				break;

			default:
				mvl |= LT_MVL_9;
				break;
			}
		}

		if(mvl) {
			start_position = lt->position;
			s->last_change = start_position;
			rc = 1;
			Testbench_PushValue(s, lt->timeval, value);
		}

		if(lt->timebuff) {
			lt->timechangecount++;

			if(lt->timecurr) {
				lt->timecurr->next = lt->timebuff;
				lt->timecurr = lt->timebuff;
			} else {
				lt->timehead = lt->timecurr = lt->timebuff;
			}

			lt->timebuff = NULL;
		}
	}

	return(rc);
}

void lt_set_timezero(struct lt_trace* lt, lxtotime_t timeval)
{
	if(lt) {
		lt->timezero = timeval;
	}
}

