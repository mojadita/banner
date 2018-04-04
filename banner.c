/* banner.c --- program to make large character banners.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Fri Dec 22 02:48:29 EET 2017
 */

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#define F(fmt) __FILE__":%d:%s: " fmt, __LINE__, __func__

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void process(FILE *f);
void hor_line(size_t l, const char *lft, const char *rgt);

struct chrinfo {
    size_t w, h;
    char **s;
};

struct range {
	wchar_t	fst;
	wchar_t	lst;
	struct chrinfo *ci;
};

static struct chrinfo *getchrinfo(wchar_t c);

static char *c_unknown[] = {
    " ?????",
    "?     ?",
    "      ?",
    "    ??",
    "   ?",
    "",
    "   ?",
    0
};

static char *c_space[] = {
    0
};

static char *c_exclam[] = {
    "#",
    "#",
    "#",
    "#",
    "#",
    "",
    "#",
    0
};

static char *c_dquot[] = {
    "## ##",
    "## ##",
    " #  #",
    "#  #",
    0
};

static char *c_hash[] = {
    ""
    ""
    "  # #",
    "  # #",
    "#######",
    "  # #",
    "#######",
    "  # #",
    "  # #",
    0
};

static char *c_dollar[] = {
    ""
    ""
    "  # #",
    " ######",
    "# # #",
    " #####",
    "  # # #",
    "######",
    "  # #",
    0
};

static char *c_percent[] = {
    "       #",
    " ##   #",
    " ##  #",
    "    #",
    "   #",
    "  #  ##",
    " #   ##",
    "#",
    0
};

static char *c_ampers[] = {
    "  #",
    " # #",
    "  #",
    "  ##",
    " #  # #",
    "#    #",
    " #### #",
    0
};

static char *c_quote[] = {
    "##",
    "##",
    " #",
    "#",
    0
};

static char *c_lparen[] = {
    "  #",
    " #",
    "#",
    "#",
    "#",
    "#",
    " #",
    "  #",
    0
};

static char *c_rparen[] = {
    "#",
    " #",
    "  #",
    "  #",
    "  #",
    "  #",
    " #",
    "#",
    0
};

static char *c_asterisk[] = {
    "",
    "",
    " #   #",
    "  # #",
    "#######",
    "  # #",
    " #   #",
    0
};

static char *c_plus[] = {
    "",
    "",
    "   #",
    "   #",
    "#######",
    "   # ",
    "   #",
    0
};

static char *c_comma[] = {
    "",
    "",
    "",
    "",
    "",
    "##",
    "##",
    " #",
    "#",
    0
};

static char *c_minus[] = {
    "",
    "",
    "",
    "",
    "#######",
    0
};

static char *c_dot[] = {
    "",
    "",
    "",
    "",
    "",
    "##",
    "##",
    0
};

static char *c_slash[] = {
    "       #",
    "      #",
    "     #",
    "    #",
    "   #",
    "  #",
    " #",
    "#",
    0
};

static char *c_0[] = {
    "  ###",
    " #   #",
    "#   # #",
    "#  #  #",
    "# #   #",
    " #   #",
    "  ###",
    0
};

static char *c_1[] = {
    "   #   ",
    "  ##",
    " # #",
    "   #",
    "   #",
    "   #",
    " ####",
    0
};

static char *c_2[] = {
    " ####",
    "##   ##",
    "     ##",
    "    ##",
    "  ##",
    " #",
    "#######",
    0
};

static char *c_3[] = {
    " #####",
    " #   # ",
    "    #  ",
    "   ##",
    "     ##",
    "##   ##",
    "  ### ",
    0
};

static char *c_4[] = {
    "    #",
    "   ##",
    "  # #",
    " #  #",
    "#######",
    "    #",
    "   ###",
    0
};

static char *c_5[] = {
    "######",
    "#",
    "##",
    "  ###",
    "     ##",
    "##   ##",
    "  ###",
    0
};

static char *c_6[] = {
    "    ##",
    "  ##",
    " #",
    "# ###",
    "##   ##",
    "##   ##",
    "  ###",
    0
};

static char *c_7[] = {
    "#######",
    "#     #",
    "     #",
    "    #",
    "   #",
    "  ##",
    "  ##",
    0
};

static char *c_8[] = {
    "  ###",
    "##   ##",
    "##   ##",
    "  ###",
    "##   ##",
    "##   ##",
    "  ###",
    0
};

static char *c_9[] = {
    "  ###",
    "##   ##",
    "##   ##",
    " #####",
    "    #",
    "  ##",
    " ##",
    0
};

static char *c_colon[] = {
    "",
    "",
    "##",
    "##",
    "",
    "##",
    "##",
    0
};

static char *c_semicolon[] = {
    "",
    "",
    "##",
    "##",
    "",
    "##",
    "##",
    " #",
    "#",
    0
};

static char *c_less[] = {
    "    #",
    "   #",
    "  #",
    " #",
    "#",
    " #",
    "  #",
    "   #",
    "    #",
    0
};

static char *c_eq[] = {
    "",
    "",
    "",
    "#######",
    "",
    "#######",
    0
};

static char *c_greater[] = {
    "#",
    " #",
    "  #",
    "   #",
    "    #",
    "   #",
    "  #",
    " #",
    "#",
    0
};

static char *c_qmark[] = {
    " #####",
    "#     #",
    "      #",
    "    ##",
    "   #",
    "",
    "   #",
    0
};

static char *c_atsign[] = {
    " #####",
    "#     #",
    "#  ####",
    "# #   #",
    "# #####",
    "#",
    " ######",
    0
};

static char *c_A[] = {
    "   #",
    "  # #",
    " #   #",
    "#     #",
    "#######",
    "#     #",
    "#     #",
    0
};

static char *c_B[] = {
    "######",
    "#     #",
    "#     #",
    "######",
    "#     #",
    "#     #",
    "######",
    0
};

static char *c_C[] = {
    " #####",
    "#     #",
    "#",
    "#",
    "#",
    "#     #",
    " #####",
    0
};

static char *c_D[] = {
    "#####",
    "#    #",
    "#     #",
    "#     #",
    "#     #",
    "#    #",
    "#####",
    0
};

static char *c_E[] = {
    "#######",
    "#",
    "#",
    "#####",
    "#",
    "#",
    "#######",
    0
};

static char *c_F[] = {
    "#######",
    "#",
    "#",
    "#####",
    "#",
    "#",
    "#",
    0
};

static char *c_G[] = {
    " ######",
    "#      #",
    "#",
    "#   ####",
    "#      #",
    "#      #",
    " ######",
    0
};

static char *c_H[] = {
    "#      #",
    "#      #",
    "#      #",
    "########",
    "#      #",
    "#      #",
    "#      #",
    0
};

static char *c_I[] = {
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    0
};

static char *c_J[] = {
    "      #",
    "      #",
    "      #",
    "      #",
    "      #",
    "#     #",
    " #####",
    0
};

static char *c_K[] = {
    "#     #",
    "#    #",
    "#   #",
    "####",
    "#   #",
    "#    #",
    "#     #",
    0
};

static char *c_L[] = {
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    "#######",
    0
};

static char *c_M[] = {
    "#     #",
    "##   ##",
    "# # # #",
    "#  #  #",
    "#     #",
    "#     #",
    "#     #",
    0
};

static char *c_N[] = {
    "#     #",
    "##    #",
    "# #   #",
    "#  #  #",
    "#   # #",
    "#    ##",
    "#     #",
    0
};

static char *c_O[] = {
    " #####",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_P[] = {
    "######",
    "#     #",
    "#     #",
    "######",
    "#",
    "#",
    "#",
    0
};

static char *c_Q[] = {
    " #####",
    "#     #",
    "#     #",
    "#     #",
    "#   # #",
    "#    #",
    " #### #",
    0
};

static char *c_R[] = {
    "######",
    "#     #",
    "#     #",
    "######",
    "#     #",
    "#     #",
    "#     #",
    0
};

static char *c_S[] = {
    " #####",
    "#     #",
    "#",
    " #####",
    "      #",
    "#     #",
    " #####",
    0
};

static char *c_T[] = {
    "#######",
    "   #",
    "   #",
    "   #",
    "   #",
    "   #",
    "   #",
    0
};

static char *c_U[] = {
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_V[] = {
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #   #",
    "  # #",
    "   #",
    0
};

static char *c_W[] = {
    "#     #",
    "#     #",
    "#     #",
    "#  #  #",
    "# # # #",
    "##   ##",
    "#     #",
    0
};

static char *c_X[] = {
    "#     #",
    " #   #",
    "  # #",
    "   #",
    "  # #",
    " #   #",
    "#     #",
    0
};

static char *c_Y[] = {
    "#     #",
    " #   #",
    "  # #",
    "   #",
    "   #",
    "   #",
    "   #",
    0
};

static char *c_Z[] = {
    "#######",
    "     #",
    "    #",
    "   #",
    "  #",
    " #",
    "#######",
    0
};

static char *c_lbkt[] = {
    "###",
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    "###",
    0
};

static char *c_bslsh[] = {
    "#",
    " #",
    "  #",
    "   #",
    "    #",
    "     #",
    "      #",
    "       #",
    0
};

static char *c_rbkt[] = {
    "###",
    "  #",
    "  #",
    "  #",
    "  #",
    "  #",
    "  #",
    "###",
    0
};

static char *c_circunf[] = {
    "",
    "  #",
    " # #",
    "#   #",
    0
};


static char *c_undscore[] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "#######",
    0
};

static char *c_lquot[] = {
    "##",
    "##",
    "#",
    " #",
    0
};

static char *c_a[] = {
    "",
    "",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_b[] = {
    "#",
    "#",
    "######",
    "#     #",
    "#     #",
    "#     #",
    "######",
    0
};

static char *c_c[] = {
    "",
    "",
    " #####",
    "#     #",
    "#",
    "#",
    " #####",
    0
};

static char *c_d[] = {
    "      #",
    "      #",
    " ######",
    "#     #",
    "#     #",
    "#     #",
    " #######",
    0
};

static char *c_e[] = {
    "",
    "",
    " #####",
    "#     #",
    "######",
    "#",
    " ######",
    0
};

static char *c_f[] = {
    "  ##",
    " #",
    "####",
    " #",
    " #",
    " #",
    " #",
    0
};

static char *c_g[] = {
    "",
    "",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " ######",
    "      #",
    " #####",
    0
};

static char *c_h[] = {
    "#",
    "#",
    "######",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    0
};

static char *c_i[] = {
    "#",
    "",
    "#",
    "#",
    "#",
    "#",
    " ##",
    0
};

static char *c_j[] = {
    "  #",
    "",
    "  #",
    "  #",
    "  #",
    "  #",
    "  #",
    "  #",
    "##",
    0
};

static char *c_k[] = {
    "#",
    "#   #",
    "#  #",
    "#####",
    "#    #",
    "#    #",
    "#    #",
    0
};

static char *c_l[] = {
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    " ##",
    0
};

static char *c_m[] = {
    "",
    "",
    "### ##",
    "#  #  #",
    "#  #  #",
    "#  #  #",
    "#  #  #",
    0
};

static char *c_n[] = {
    "",
    "",
    "# ####",
    "##    #",
    "#     #",
    "#     #",
    "#     #",
    0
};

static char *c_o[] = {
    "",
    "",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_p[] = {
    "",
    "",
    "######",
    "#     #",
    "#     #",
    "#     #",
    "######",
    "#",
    "#",
    0
};

static char *c_q[] = {
    "",
    "",
    " ######",
    "#     #",
    "#     #",
    "#     #",
    " ######",
    "      #",
    "      #",
    0
};

static char *c_r[] = {
    "",
    "",
    "# #####",
    "##    #",
    "#",
    "#",
    "#",
    0
};

static char *c_s[] = {
    "",
    "",
    " ######",
    "#",
    " #####",
    "      #",
    "######",
    0
};

static char *c_t[] = {
    "  #",
    "  #",
    "######",
    "  #",
    "  #",
    "  #",
    "   ###",
    0
};

static char *c_u[] = {
    "",
    "",
    "#    #",
    "#    #",
    "#    #",
    "#    #",
    " #### #",
    0
};

static char *c_v[] = {
    "",
    "",
    "#     #",
    "#     #",
    " #   #",
    "  # #",
    "   #",
    0
};

static char *c_w[] = {
    "",
    "",
    "#     #",
    "#     #",
    "#  #  #",
    "#  #  #",
    " ## ##",
    0
};

static char *c_x[] = {
    "",
    "",
    "#     #",
    " #   #",
    "   #",
    " #   #",
    "#     #",
    0
};

static char *c_y[] = {
    "",
    "",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " ######",
    "      #",
    " #####",
    0
};

static char *c_z[] = {
    "",
    "",
    "########",
    "      #",
    "   ##",
    " #",
    "########",
    0
};

static char *c_lcbkt[] = {
    "  ##",
    " #",
    " #",
    " #",
    "#",
    " #",
    " #",
    " #",
    "  ##",
    0
};

static char *c_vbar[] = {
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    "#",
    0
};

static char *c_rcbkt[] = {
    "##",
    "  #",
    "  #",
    "  #",
    "   #",
    "  #",
    "  #",
    "  #",
    "##",
    0
};

static char *c_tilde[] = {
    " ###  #",
    "#   ##",
    0
};

static struct chrinfo  ci_invalid[] = {
	{0, 0, c_unknown,},
};

static struct chrinfo  latin1_0[] = {
    {7,0,c_space},
	{0,0,c_exclam}, {0,0,c_dquot}, {0,0,c_hash},
	{0,0,c_dollar}, {0,0,c_percent}, {0,0,c_ampers},
	{0,0,c_quote}, {0,0,c_lparen}, {0,0,c_rparen},
	{0,0,c_asterisk}, {0,0,c_plus}, {0,0,c_comma},
	{0,0,c_minus}, {0,0,c_dot}, {0,0,c_slash},
	{0,0,c_0}, {0,0,c_1}, {0,0,c_2}, {0,0,c_3},
    {0,0,c_4}, {0,0,c_5}, {0,0,c_6}, {0,0,c_7},
	{0,0,c_8}, {0,0,c_9}, {0,0,c_colon},
	{0,0,c_semicolon}, {0,0,c_less}, {0,0,c_eq},
	{0,0,c_greater}, {0,0,c_qmark}, {0,0,c_atsign},
	{0,0,c_A}, {0,0,c_B}, {0,0,c_C}, {0,0,c_D},
	{0,0,c_E}, {0,0,c_F}, {0,0,c_G}, {0,0,c_H},
	{0,0,c_I}, {0,0,c_J}, {0,0,c_K}, {0,0,c_L},
	{0,0,c_M}, {0,0,c_N}, {0,0,c_O}, {0,0,c_P},
	{0,0,c_Q}, {0,0,c_R}, {0,0,c_S}, {0,0,c_T},
	{0,0,c_U}, {0,0,c_V}, {0,0,c_W}, {0,0,c_X},
	{0,0,c_Y}, {0,0,c_Z}, {0,0,c_lbkt},
	{0,0,c_bslsh}, {0,0,c_rbkt}, {0,0,c_circunf},
	{0,0,c_undscore}, {0,0,c_lquot}, {0,0,c_a},
	{0,0,c_b}, {0,0,c_c}, {0,0,c_d}, {0,0,c_e},
	{0,0,c_f}, {0,0,c_g}, {0,0,c_h}, {0,0,c_i},
	{0,0,c_j}, {0,0,c_k}, {0,0,c_l}, {0,0,c_m},
	{0,0,c_n}, {0,0,c_o}, {0,0,c_p}, {0,0,c_q},
	{0,0,c_r}, {0,0,c_s}, {0,0,c_t}, {0,0,c_u},
	{0,0,c_v}, {0,0,c_w}, {0,0,c_x}, {0,0,c_y},
	{0,0,c_z}, {0,0,c_lcbkt}, {0,0,c_vbar},
	{0,0,c_rcbkt}, {0,0,c_tilde},
};

struct range ranges[] = {
	{ 0xfffe, 0xffff, ci_invalid },
	{ ' ', 0x7f, latin1_0 },

	{ 0, 0, NULL },
};

static struct chrinfo *getchrinfo(wchar_t c)
{
	struct range *p;

	for (p = ranges; p->ci; p++) {
		if (p->fst <= c & c < p->lst)
			break;
	}

	return p->ci
		? p->ci + (c - p->fst)
		: ci_invalid;
} /* getchrinfo */

int flags = 0;
#define FLAG_MONOSP (1 << 0)
#define FLAG_FRAME  (1 << 1)
int max_width = 0;

int main(int argc, char **argv)
{
    int opt;

	setlocale(LC_ALL, "");

    while ((opt = getopt(argc, argv, "fm")) != EOF) {
        switch(opt) {
        case 'f': flags |= FLAG_FRAME; break;
        case 'm': flags |= FLAG_MONOSP; break;
        } /* switch */
    } /* while */
    argc -= optind; argv += optind;

    /* initialize tables */
    struct range *r;
    for (r = ranges; r->ci; r++) {
		int c, i;
		for (i = 0, c = r->fst; c < r->lst; i++, c++) {
			int j;

			for (j = 0; r->ci[i].s[j]; j++) {
				int n = strlen(r->ci[i].s[j]);
				if (r->ci[i].w < n) r->ci[i].w = n;
			} /* for */
			if (max_width < r->ci[i].w)
				max_width = r->ci[i].w;
			r->ci[i].h = j;
		} /* for */
    } /* for */

    if (argc) {
		int i;
        for (i = 0; i < argc; i++) {
            FILE *f = fopen(argv[i], "r");
            if (!f) {
                fprintf(stderr,
                        F("fopen: %s: %s (errno = %d)\n"),
                        argv[i], strerror(errno), errno);
                exit(EXIT_FAILURE);
            } /* if */
            process(f);
            fclose(f);
        }
    } else {
        process(stdin);
    } /* else */
} /* main */

void process(FILE *f)
{
	static long lineno = 0;
    wchar_t line[BUFSIZ];
    size_t last_l = 0, this_l = 0;

    while (fgetws(line, sizeof line, f)) {
		wchar_t *ctx;
        wchar_t *l = wcstok(line, L"\n", &ctx);
        if (!l) l = L"";
        size_t len = wcslen(l);
        this_l = len ? (len - 1) * 2 : 0;
        int i, h = 7;

        for (i = 0; i < len; i++) {
            struct chrinfo *p = getchrinfo(l[i]);
            if (h < p->h) h = p->h;
            this_l += flags & FLAG_MONOSP
				? max_width
				: p->w;
        } /* for */
            
        if (flags & FLAG_FRAME && (last_l || this_l)) {
            if (last_l == 0) {
                hor_line(this_l, ",=", "=.\n");
            } else if (this_l == 0) {
                hor_line(last_l, "`=", "='\n");
            } else if (last_l == this_l) {
                hor_line(last_l, ">=", "=<\n");
            } else { /* last_l != this_l, both != 0 */
                size_t min = MIN(last_l, this_l);
                size_t max = MAX(last_l, this_l);
                hor_line(min + 1,
                    ">=", last_l > this_l
                        ? "v"
                        : "^");
                hor_line(max - min - 1,
                    "", last_l > this_l
                        ? "'\n"
                        : ".\n");
            } /* else */
        } else {
            if (lineno++) puts("");
        } /* if */

        for (i = 0; i < h; i++) {
            int j;
            if (flags & FLAG_FRAME && len)
                printf("| ");
            for (j = 0; j < len; j++) {
                struct chrinfo *p = getchrinfo(l[j]);
                int pre1 = j 
                        ? 2
                        : 0,
                    pre2 = flags & FLAG_MONOSP
                        ? (max_width - p->w) >> 1
                        : 0;
                printf("%*s%-*s",
                        pre1 + pre2, "",
                        flags & FLAG_MONOSP
                            ? max_width - pre2
                            : p->w,
                        i < p->h
                            ? p->s[i]
                            : "");
            } /* for */
            puts(flags & FLAG_FRAME && len ? " |" : "");
        } /* for */
        last_l = this_l;
    } /* while */
    if (flags & FLAG_FRAME && last_l) {
        hor_line(this_l, "`=", "='\n");
    } /* if */
} /* process */

void hor_line(size_t len, const char *lft, const char *rgt)
{
    static char the_line[]=
"=========================================================";
    static size_t the_line_size = sizeof the_line - 1;
    printf("%s", lft);
    while (len > the_line_size) {
        printf("%s", the_line);
        len -= the_line_size;
    } /* while */
    /* len <= the_line_size */
    printf("%.*s%s", len, the_line, rgt);
} /* hor_line */
