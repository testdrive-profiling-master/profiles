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

#ifndef DEFS_LXT_H
#define DEFS_LXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <zlib.h>
#include <bzlib.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifndef HAVE_FSEEKO
#define fseeko fseek
#define ftello ftell
#endif


typedef enum {
	V_EVENT, V_PARAMETER,
	V_INTEGER, V_REAL, V_REAL_PARAMETER = V_REAL, V_REALTIME = V_REAL, V_STRINGTYPE = V_REAL, V_REG, V_SUPPLY0,
	V_SUPPLY1, V_TIME, V_TRI, V_TRIAND, V_TRIOR,
	V_TRIREG, V_TRI0, V_TRI1, V_WAND, V_WIRE, V_WOR, V_PORT, V_IN, V_OUT, V_INOUT,
	V_END, V_LB, V_COLON, V_RB, V_STRING
} VarTypes;

typedef struct dslxt_tree_node dslxt_Tree;
struct dslxt_tree_node {
    dslxt_Tree * left, * right;
    char *item;
    unsigned int val;
};


#define LT_HDRID (0x0138)
#define LT_VERSION (0x0004)
#define LT_TRLID (0xB4)

#define LT_CLKPACK (4)
#define LT_CLKPACK_M (2)

#define LT_MVL_2	(1<<0)
#define LT_MVL_4	(1<<1)
#define LT_MVL_9	(1<<2)

#define LT_MINDICTWIDTH (16)

#ifndef _MSC_VER
typedef uint64_t                lxttime_t;
#define ULLDescriptor(x) x##ULL
typedef int64_t                 lxtotime_t;
#else
typedef unsigned __int64        lxttime_t;
#define ULLDescriptor(x) x##i64
typedef          __int64        lxtotime_t;
#endif


struct lt_timetrail
{
struct lt_timetrail *next;
lxttime_t timeval;
unsigned int position;
};


#define LT_SYMPRIME 500009

#define LT_SECTION_END				(0)
#define LT_SECTION_CHG				(1)
#define LT_SECTION_SYNC_TABLE			(2)
#define LT_SECTION_FACNAME			(3)
#define LT_SECTION_FACNAME_GEOMETRY		(4)
#define LT_SECTION_TIMESCALE			(5)
#define	LT_SECTION_TIME_TABLE			(6)
#define LT_SECTION_INITIAL_VALUE		(7)
#define LT_SECTION_DOUBLE_TEST			(8)
#define	LT_SECTION_TIME_TABLE64			(9)
#define LT_SECTION_ZFACNAME_PREDEC_SIZE		(10)
#define LT_SECTION_ZFACNAME_SIZE		(11)
#define LT_SECTION_ZFACNAME_GEOMETRY_SIZE 	(12)
#define LT_SECTION_ZSYNC_SIZE			(13)
#define LT_SECTION_ZTIME_TABLE_SIZE		(14)
#define LT_SECTION_ZCHG_PREDEC_SIZE		(15)
#define LT_SECTION_ZCHG_SIZE			(16)
#define LT_SECTION_ZDICTIONARY			(17)
#define LT_SECTION_ZDICTIONARY_SIZE		(18)
#define LT_SECTION_EXCLUDE_TABLE		(19)
#define LT_SECTION_TIMEZERO			(20)

struct lt_trace {
	unsigned int mindictwidth;

	unsigned int position;

	struct lt_symbol *sym[LT_SYMPRIME];
	struct lt_symbol **sorted_facs;
	struct lt_symbol *symchain;
	int numfacs, numfacs_bytes;
	int numfacbytes;
	int longestname;
	lxttime_t mintime, maxtime;
	int timescale;
	int initial_value;

	struct lt_timetrail *timehead, *timecurr, *timebuff;
	int timechangecount;

	unsigned int facname_offset;
	unsigned int facgeometry_offset;
	unsigned int sync_table_offset;
	unsigned int initial_value_offset;
	unsigned int timescale_offset;
	unsigned int exclude_offset;
	unsigned int timezero_offset;

	lxttime_t		timeval; 			// for clock induction, current time
	lxtotime_t		timezero;			// for allowing negative values

	unsigned double_used : 1;
	unsigned do_strip_brackets : 1;
	unsigned clock_compress : 1;
	unsigned emitted : 1;				// gate off change field zmode changes when set
};

class vTestbenchVar;
struct lt_symbol
{
	struct lt_symbol *next;
	struct lt_symbol *symchain;
	char *name;
	int namlen;

	int facnum;
	struct lt_symbol *aliased_to;

	unsigned int rows;
	int msb, lsb;
	int len;
	int flags;

	unsigned int last_change;

	lxttime_t	clk_delta;
	lxttime_t	clk_prevtrans;
	int		clk_numtrans;
	int 		clk_prevval;
	int 		clk_prevval1;
	int 		clk_prevval2;
	int 		clk_prevval3;
	int 		clk_prevval4;
	unsigned char	clk_mask;

	unsigned char			vartype;

	vTestbenchVar*	pTestbench;
};

#define LT_SYM_F_BITS           (0)
#define LT_SYM_F_INTEGER        (1<<0)
#define LT_SYM_F_DOUBLE         (1<<1)
#define LT_SYM_F_STRING         (1<<2)
#define LT_SYM_F_ALIAS          (1<<3)


struct lt_trace *	lt_init(void);
void 			lt_close(struct lt_trace *lt);

struct lt_symbol *	lt_symbol_find(struct lt_trace *lt, const char *name);
struct lt_symbol *	lt_symbol_add(struct lt_trace *lt, struct vcdsymbol* v, const char *name, unsigned int rows, int msb, int lsb, int flags);
struct lt_symbol *	lt_symbol_alias(struct lt_trace *lt, struct vcdsymbol* v, const char *existing_name, const char *alias, int msb, int lsb);

void 			lt_set_clock_compress(struct lt_trace *lt);
void 			lt_set_initial_value(struct lt_trace *lt, char value);
void 			lt_set_timescale(struct lt_trace *lt, int timescale);
void			lt_set_timezero(struct lt_trace *lt, lxtotime_t timeval);

int 			lt_set_time(struct lt_trace *lt, unsigned int timeval);
int 			lt_inc_time_by_delta(struct lt_trace *lt, unsigned int timeval);
int 			lt_set_time64(struct lt_trace *lt, lxttime_t timeval);

/*
 * value change functions..note that if the value string len for
 * lt_emit_value_bit_string() is shorter than the symbol length
 * it will be left justified with the rightmost character used as
 * a repeat value that will be propagated to pad the value string out:
 *
 * "10x" for 8 bits becomes "10xxxxxx"
 * "z" for 8 bits becomes   "zzzzzzzz"
 */
int 			lt_emit_value_int(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, int value);
int 			lt_emit_value_double(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, double value);
int 			lt_emit_value_string(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, char *value);
int 			lt_emit_value_bit_string(struct lt_trace *lt, struct lt_symbol *s, unsigned int row, char *value);

#ifdef __cplusplus
}
#endif

#endif

