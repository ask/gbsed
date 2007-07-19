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
* $Id: libgbsed.c,v 1.2 2007/07/14 14:58:07 ask Exp $
* $Source: /opt/CVS/File-BSED/libgbsed.c,v $
* $Author: ask $
* $HeadURL$
* $Revision: 1.2 $
* $Date: 2007/07/14 14:58:07 $
*/

#ifndef PACKAGE
#  define PACKAGE "gbsed"
#endif

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#ifdef HAVE_LIBGEN_H
#  include <libgen.h>
# else /* not HAVE_LIBGEN_H */
#   define HAVE_LIBGEN_H 0
#endif /* HAVE_LIBGEN_H */

#include <libgbsed.h>

/* error code from libgbsed.h */
extern int  gbsed_errno;
extern int  gbsed_warnings[];
extern int  gbsed_warn_index;

/* Helper functions. */
void print_usage(char *, char *);
void print_version(void);
void print_help(char *);

/* Our arguments...  */
static struct option longopts[] =
{
    { "search",      required_argument, NULL, 's' }, 
    { "replace",     required_argument, NULL, 'r' },
    { "searchstr",   required_argument, NULL, 'S' }, 
    { "replacestr",  required_argument, NULL, 'R' }, 
    { "min",         required_argument, NULL, 'm' }, 
    { "max",         required_argument, NULL, 'M' }, 
    { "dump",        required_argument, NULL, 'D' }, 
    { "help",        no_argument,       NULL, 'h' }, 
    { "version",     no_argument,       NULL, 'v' }, 
    { "silent",      no_argument,       NULL, 'x' }, 
    { "very-silent", no_argument,       NULL, 'X' }, 
}; 


/* Short version of the arguments */
const char *short_arglist = "xXhvs:r:S:R:m:M:D:";

/* Usage information. */
const char usage_1[]
    = "[-m <min>|-M <max>|-x|-X] [-s|-S] <search for> infile";
const char usage_2[]
    = "[-m <min>|-M <max>|-x|-X] [-s|-S] <search for> [-r|-R] <replace with> infile outfile";
const char usage_3[]
    = "[--help|--version]";
const char usage_4[]
    = "-D <text>";

/* Copyright and license information. */
const char *license_info  = {
    "Copyright (C) 2007 Ask Solem <ask@0x61736b.net>\n"
    "License GPLv3+: GNU GPL version 3 or later "
    "<http://gnu.org/licenses/gpl.html>\n"
    "This is free software: you are free to change and distribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.\n\n"
};

#define _myfree()   do {                                \
    if (replace_allocated)  _gbsed_safefree(replace);   \
    if (search_allocated)   _gbsed_safefree(search);    \
    if (args_allocated)     _gbsed_safefree(gbsedargs); \
    } while(0)

int main(int argc, char **argv)
{
    int   ch;                   /* return for getopt    */
    int   minmatch =  1;        /* minimum matches      */
    int   maxmatch = -1;        /* maximum matches      */
    char *search   = NULL;      /* search  string       */
    char *replace  = NULL;      /* replace string       */
    char *infile   = NULL;      /* input   file name    */
    char *outfile  = NULL;      /* output  file name    */
    char *myself   = NULL;      /* this program name    */
    int   match    = 0;         /* number of matches    */
    int   sysret;               /* exit status          */
    int   i        = 0;         /* std. temp iterator   */
    
    GBSEDargs  *gbsedargs;      /* Arguments to gbsed   */
    const char *gbsed_errmsg;   /* error from gbsed     */
    
    int  opt_silent  = 0;       /* don't print warnings */
    int  opt_vsilent = 0;       /* don't print warnings and errors */

    /* poor mans garbage collection :-) */
    int replace_allocated = 0;
    int search_allocated  = 0;
    int args_allocated    = 0;

    /* everything is OK until proven wrong. */
    sysret    = EXIT_SUCCESS;
    gbsedargs = NULL;
   
    /* get the file part of the path in argv[0] */ 
    if (HAVE_LIBGEN_H) 
        myself = basename(argv[0]);
    else
        myself = argv[0];

    /* Get program arguments. */
    while ((ch = getopt_long(argc, argv, short_arglist, longopts, NULL)) != -1)
    {
        switch (ch) {
            case 'h':
                print_help(myself);
                break;
            case 'v':
                print_version();
                break;
            case 's':
                search  = strdup(optarg);
                search_allocated++; 
                break;
            case 'S':
                search = gbsed_string2hexstring(optarg);
                search_allocated++;
                break;
            case 'r':
                replace = strdup(optarg);
                replace_allocated++;
                break;
            case 'R':
                replace = gbsed_string2hexstring(optarg);
                replace_allocated++;
                break;
            case 'm':
                minmatch = atoi(optarg);
                if (minmatch <= 0)
                    print_usage(myself, "Minmatch must be greater than 0.");
                break;
            case 'M':
                maxmatch = atoi(optarg);
                if (maxmatch <= 0)
                    print_usage(myself, "Maxmatch must be greater than 0.");
                break;
            case 'D':
                search = gbsed_string2hexstring(optarg);
                printf("%s\n", search);
                goto CLEANUP;
            case 'x':
                opt_silent++;
                break;
            case 'X':
                opt_silent++;
                opt_vsilent++;
                break;

            default:
                print_usage(myself, NULL);
        }
    }
    argc -= optind;
    argv += optind;

    /* Need atleast search string and input file */
    if (search == NULL)
        print_usage(myself, "Missing search string.");
    if (argc < 1)
        print_usage(myself, "Missing input file.");
    infile = argv[0];

    /* Replace is optional. */
    if (replace != NULL) {

        /* if replace is set, we need a file name to output data to. */
        if (argc < 2)
            print_usage(myself, "Replace is on, but missing output file.");

        outfile = argv[1];
    }
    if (argc > 1)
        outfile = argv[1];

    /* Allocate memory for the gbsed_binary_search_rep*() argument struct. */    
    gbsedargs     = _gbsed_alloc(gbsedargs, 1, GBSEDargs);
    if (gbsedargs == NULL) {
        if (!opt_vsilent)
            fprintf(stderr, "ERROR: Out of memory!\n");
        goto CLEANUP;
    }
    args_allocated++;
   
    /* Populate the argument struct. */ 
    gbsedargs->search        = search;
    gbsedargs->replace       = replace;
    gbsedargs->infilename    = infile;
    gbsedargs->outfilename   = outfile;
    gbsedargs->minmatch      = minmatch;
    gbsedargs->maxmatch      = maxmatch;

    /* Do the search(/replace). */
    match = gbsed_binary_search_replace(gbsedargs);

    if (gbsed_warn_index > 0) {
        for (i = 0; i < gbsed_warn_index; i++)
            if (!opt_silent)
                fprintf(stderr, "WARNING: %s\n", gbsed_warntostr(gbsed_warnings[i]));
    }

    switch (match) {

        case GBSED_ERROR:
            /* Error from libgbsed. */
            gbsed_errmsg = gbsed_errtostr(gbsed_errno);
            if (!opt_vsilent)
                fprintf(stderr, "ERROR: %s\n", gbsed_errmsg);
            sysret = EXIT_FAILURE;
            break;

        case GBSED_NO_MATCH:
            /* No match was found. */
            if (!opt_vsilent)
                fprintf(stderr, "0 (No match).\n");
            sysret = EXIT_FAILURE;
            break;

        default:
            /* Matches found. */
            if (!opt_vsilent)
                fprintf(stderr,"%d (%s)\n", match,
                    (match == 1 ? "match" : "matches")
                );
            break;
    }

    CLEANUP: 
    _myfree();
    return sysret;
}


void
print_version(void)
{
    fprintf(stdout, "\n%s %s\n\n", PACKAGE, gbsed_version());
    fprintf(stdout, "%s", license_info);
    exit   (EXIT_SUCCESS);
}

void
print_help(char *program_name)
{

    const char help_description[] = {
        "DESCRIPTION\n\n"
        "gbsed lets you search and replace binary strings "
        "and text strings in binary files.\nIf no replace "
        "string is given, it will only print out the number "
        "of matches found in the file.\n\n"
        "Standard in/out will be used if the input/output file "
        "name argument is '-'.\n"
    };

    const char help_options1[]     = {
        "OPTIONS\n\n"
        "\t-s, --search            A string of hex values to search for.\n"
        "\t-r, --replace           A string of hex values to replace with.\n"
        "\t-S, --searchstr         Search for string instead of hex.\n"
        "\t-R, --replacestr        Replace with a string.\n"
        "\t-m, --min               Need atleast these many matches.\n"
        "\t-M, --max               Stop at a number of matches.\n"
    };

    const char help_options2[]     = {
        "\t-x, --silent            Suppress warnings.\n"
        "\t-X, --very-silent       Suppress all output.\n"
        "\t-D, --dump              Dump the hex version of a text string.\n"
        "\t-v, --version           Print version information and exit.\n"
        "\t-h, --help              This help screen.\n"
    };
   
    const char help_wildcards[]     = {
        "WILDCARDS\n\n"
	    "Wildcard bytes (\'\?\?\') are supported in both the search and replace strings\n"
	    "Both search and replace strings must be multiples of whole bytes,\n"
	    "Only the characters 0-9, a-f, A-F and ? are acceptible\n\n"
    };

    const char help_examples[]      = {
        "EXAMPLES\n"
        "\tgbsed --search 0xffc3 /bin/ls\n"
        "\tgbsed --search 0xffc3 --replace 0x00ea /bin/ls ./ls.patched\n"
        "\tgbsed --search --min 3 --max 12000 0xff /bin/ls\n"
        "\tgbsed --searchstr HELLO /bin/ls\n"
        "\tcat /bin/ls | gbsed -S connected -R corrupted - - | od -cx\n"
        "\tgbsed --dump 'hello world'"
    };

    const char see_also[]           = {
        "SEE ALSO\n\n"
        "\tman 1 gbsed\n"
        "\tman 3 libgbsed\n"
    };
    fprintf(stderr, "\n%s v%s\n\n",
        program_name, gbsed_version()
    );

    fprintf(stdout, "%s\n", help_description);

    fprintf(stdout, "USAGE\n");
    fprintf(stdout, "\t%s %s\n",   program_name, usage_1);
    fprintf(stdout, "\t%s %s\n",   program_name, usage_2);
    fprintf(stdout, "\t%s %s\n", program_name, usage_3);
    fprintf(stdout, "\t%s %s\n\n", program_name, usage_4);

    fprintf(stdout, "%s\n", help_options1);
    fprintf(stdout, "%s\n", help_options2);
    fprintf(stdout, "%s\n", help_wildcards);

    fprintf(stdout, "%s\n\n", help_examples);
    fprintf(stdout, "%s\n", see_also);

    fprintf(stdout, "%s\n", license_info);

    fprintf(stdout, "Report bug reports to <bug-file-bsed@rt.cpan.org>\n\n");

    exit (EXIT_SUCCESS);
}


void
print_usage(char *program_name, char *error_message)
{
    fprintf(stderr, "\n%s v%s\n\n",    program_name, gbsed_version());
    fprintf(stderr, "Usage:\t%s %s\n", program_name, usage_1);
    fprintf(stderr, "\t%s %s\n",       program_name, usage_2);
    fprintf(stderr, "\t%s %s\n",     program_name, usage_3);
    fprintf(stderr, "\t%s %s\n\n",     program_name, usage_4);
    
    if (error_message != NULL) 
        fprintf(stderr, "\nERROR: %s\n", error_message);

    exit (EXIT_FAILURE);
}

/*

=pod

=head1 NAME

gbsed - Search/replace in binary files.

=head1 VERSION

This document describes gbsed version 1.0.

=head1 SYNOPSIS

    gbsed [-m <min>|-M <max>|-x|-X] [-s|-S] <search for> infile
    gbsed [-m <min>|-M <max>|-x|-X] [-s|-S] <search for> [-r|-R] <replace with> infile outfile
    gbsed -D <text>
    gbsed [--help|--version

=head1 DESCRIPTION

gbsed lets you search and replace binary strings
and text strings in binary files.\nIf no replace
string is given, it will only print out the number
of matches found in the file.

you can search by using hex values in text strings, you can also use
wildcard matches (C<??>), which will match any wide byte.

Wildcards can not be used with the text search.

Standard in/out will be used if the input/output file
name argument is C<->.

These are all valid search strings:

    --search "0xffc300193ab2f63a"
    --search "0xff??00??3ab2f??a"
    --search "FF??00??3AB2F??A"

while these are not:

    --search "the quick brown fox" # must use --searchstr option for text
    --search "0xff?c33ab3?accc"    # no nybbles only wide bytes. (??  not ?).

=head1 OPTIONS

=head2 C<--search, -s>

A string of hex values to search for.

=head2 C<-r, --replace>

A string of hex values to replace with.

=head2 C<-S, --searchstr>

Search for string instead of hex.

=head2 C<-R, --replacestr>

Replace with a string.

=head2 C<-m, --min>

Need atleast these many matches.

=head2 C<-M, --max>

Stop at a number of matches.

=head2 C<-D, --dump>

Dump hex representation of a text string (shows what is done to a -S or -R argument).

=head2 C<-x, --silent>

Suppress warnings.

=head2 C<-X, --very-silent>

Suppress all output.

=head2 C<-v, --version>

Print version information and exit.

=head2 C<-h, --help>

This help screen.

=head1 EXAMPLES

        gbsed --search 0xffc3 /bin/ls
        gbsed --search 0xffc3 --replace 0x00ea /bin/ls ./ls.patched
        gbsed --search --min 3 --max 12000 0xff /bin/ls
        gbsed --searchstr HELLO /bin/ls
        cat /bin/ls | gbsed -x -S connected -R corrupted - - | od -cx
        gbsed -D "hello world"

=head1 CONFIGURATION AND ENVIRONMENT

C<libgbsed> requires no configuration file or environment variables.

=head1 INCOMPATIBILITIES

None known.

=head1 BUGS AND LIMITATIONS

No bugs have been reported.

Please report any bugs or feature requests to
C<bug-file-bsed@rt.cpan.org>, or through the web interface at
L<http://rt.cpan.org>.

=head1 SEE ALSO

=over 4

=item * L<libgbsed>

=back

=head1 AUTHOR

Ask Solem,   C<< ask@0x61736b.net >>.

=head1 ACKNOWLEDGEMENTS

Dave Dykstra C<< dwdbsed@drdykstra.us >>.
for C<bsed> the original program,

I<0xfeedface>
for the wildcards patch.

=head1 LICENSE AND COPYRIGHT

Copyright (C) 2007 Ask Solem <ask@0x61736b.net>

gbsed is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

gbsed is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

=head1 DISCLAIMER OF WARRANTY

BECAUSE THIS SOFTWARE IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR THE
SOFTWARE, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE
STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE
SOFTWARE "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND
PERFORMANCE OF THE SOFTWARE IS WITH YOU. SHOULD THE SOFTWARE PROVE DEFECTIVE,
YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR, OR CORRECTION.

IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY
COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE
SOFTWARE AS PERMITTED BY THE ABOVE LICENCE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE SOFTWARE (INCLUDING BUT NOT LIMITED TO
LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR
THIRD PARTIES OR A FAILURE OF THE SOFTWARE TO OPERATE WITH ANY OTHER
SOFTWARE), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

=cut

*/

/*
# Local Variables:
#   mode: cperl
#   cperl-indent-level: 4
#   fill-column: 78
# End:
# vim: expandtab tabstop=4 shiftwidth=4 shiftround
*/

