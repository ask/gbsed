/*
*
*    Copyright (C) 2007 Ask Solem <ask@0x61736b.net>
*
*    This file is part of gbsed
*
*    gbsed is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 3 of the License, or
*    (at your option) any later version.
*
*    gbsed is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
* $Id: libgbsed.h,v 1.1 2007/07/14 14:39:49 ask Exp $
* $Source: /opt/CVS/File-gbsed/libgbsed.h,v $
* $Author: ask $
* $HeadURL$
* $Revision: 1.1 $
* $Date: 2007/07/14 14:39:49 $
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef _LIBGBSED_H_
#define _LIBGBSED_H_


/*           Constants             */

#define GBSED_ERROR                 -1
#define GBSED_NO_MATCH             0x0

#define GBSED_ESEARCH_TOO_LONG     0x1
#define GBSED_ENULL_SEARCH         0x2
#define GBSED_EREPLACE_TOO_LONG    0x3
#define GBSED_ENULL_REPLACE        0x4
#define GBSED_EMISSING_INPUT       0x5
#define GBSED_EMISSING_OUTPUT      0x6
#define GBSED_EINVALID_CHAR        0x7 
#define GBSED_ENIBBLE_NOT_BYTE     0x8 
#define GBSED_EOPEN_INFILE         0x9
#define GBSED_EOPEN_OUTFILE        0xa
#define GBSED_ENOMEM               0xb
#define GBSED_EMINMAX_BALANCE      0xc
#define GBSED_ENOSTAT_FDES          0xd

#define GBSED_WBALANCE             0x1

#define GBSED_MAXMATCH_NO_LIMIT     -1

/*           Types                 */

typedef unsigned char UCHAR;

struct gbsed_arguments {
    char *search;
    char *replace;
    char *infilename;
    char *outfilename;
    int  minmatch;
    int  maxmatch;
};
typedef struct gbsed_arguments  GBSEDargs;

struct fgbsed_arguments {
    char *search;
    char *replace;
    FILE *infile;
    FILE *outfile;
    int   minmatch;
    int   maxmatch;
};
typedef struct fgbsed_arguments fGBSEDargs;

/*           Public functions.      */

const char*
gbsed_version(void);

int
gbsed_binary_search_replace(struct gbsed_arguments *);

int
gbsed_fbinary_search_replace(struct fgbsed_arguments *);

char *
gbsed_string2hexstring(char *orig);

const char*
gbsed_errtostr(int);

char*
gbsed_warntostr(int);


void *
_gbsed_alloczero(size_t,  size_t);




#ifdef PERL_MALLOC
#  include <EXTERN.h>
#  include <perl.h>
#  define _gbsed_alloc(pointer, add, type)   \
    (type *)Newxz(pointer, add, type)
#  define  _gbsed_realloc(pointer, add, type) \
    (type *)Renew(pointer, add, type)
#  define _gbsed_safefree(pointer)           \
    Safefree(pointer)
#else /* not PERL_MALLOC */
#define _gbsed_alloc(pointer, add, type)    \
    (type *)_gbsed_alloczero(add, sizeof(type))
#define _gbsed_realloc(pointer, add, type)  \
    (type *)realloc(pointer, add*sizeof(type))
#define _gbsed_safefree(pointer)            \
    free(pointer)
#endif /* PERL_MALLOC */

/*           Private functions       */

#ifdef LIBGBSED_PRIVATE

char *
_gbsed_remove_0x_from_str(char *);

UCHAR *
_gbsed_hexstr2bin(register UCHAR *, int *);

mode_t
_gbsed_preserve_execbit(FILE *file);

#else  /* not lIBGBSED_PRIVATE */
#  define LIBGBSED_PRIVATE 0
#endif /*     LIBGBSED_PRIVATE */

#endif /* _LIBGBSED_H_ */


/*
# Local Variables:
#   indent-level: 4
#   fill-column: 78
# End:
# vim: expandtab tabstop=4 shiftwidth=4 shiftround
*/
