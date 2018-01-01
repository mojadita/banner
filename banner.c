/* banner.c --- program to make large character banners.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Fri Dec 22 02:48:29 EET 2017
 */

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F(fmt) __FILE__":%d:%s: " fmt, __LINE__, __func__

void process(FILE *f);

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
    " #####",
    "#    ##",
    "#   # #",
    "#  #  #",
    "# #   #",
    "##    #",
    " #####",
    0
};

static char *c_1[] = {
    "   #   ",
    "  ##",
    "   #",
    "   #",
    "   #",
    "   #",
    "  ###",
    0
};

static char *c_2[] = {
    " #####",
    "#     #",
    "      #",
    " #####",
    "#",
    "#",
    "#######",
    0
};

static char *c_3[] = {
    " #####",
    " #   # ",
    "    #  ",
    "   ###",
    "      #",
    "#     #",
    " #####",
    0
};

static char *c_4[] = {
    "   ##",
    "  # #",
    " #  #",
    "#   #",
    "#######",
    "    #",
    "    #",
    0
};

static char *c_5[] = {
    "######",
    "#",
    "#",
    "######",
    "      #",
    "#     #",
    " #####",
    0
};

static char *c_6[] = {
    "   ###",
    "  #",
    " #",
    "######",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_7[] = {
    " ######",
    " #    #",
    "     #",
    "    #",
    "   #",
    "   #",
    "   #",
    0
};

static char *c_8[] = {
    " #####",
    "#     #",
    "#     #",
    " #####",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_9[] = {
    " #####",
    "#     #",
    "#     #",
    " #####",
    "    #",
    "   #",
    "  #",
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
    "#  #  #",
    "#  #  #",
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

static struct chrinfo {
    size_t w, h;
    char **s;
} chars[] = {
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

    {0,0,c_unknown}
};
static size_t chars_n = (sizeof chars / sizeof chars[0]) - 1;

int flags = 0;
#define FLAG_MONOSP (1 << 0)
int max_width = 0;

int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "m")) != EOF) {
        switch(opt) {
        case 'm': flags |= FLAG_MONOSP; break;
        } /* switch */
    } /* while */
    argc -= optind; argv += optind;

    /* initialize table */
    int i;
    for (i = 0; i <= chars_n; i++) {
        int j;

        for (j = 0; chars[i].s[j]; j++) {
            int n = strlen(chars[i].s[j]);
            if (chars[i].w < n) chars[i].w = n;
        } /* for */
        if (max_width < chars[i].w)
            max_width = chars[i].w;
        chars[i].h = j;
    } /* for */

    if (argc) {
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

static struct chrinfo *getchrinfo(unsigned char c)
{
    c -= ' ';
    return chars + (c < chars_n ? c : chars_n);
} /* getchrinfo */

void process(FILE *f)
{
	static long lineno = 0;
    unsigned char line[BUFSIZ];

    while (fgets((char *)line, sizeof line, f)) {
        char *l = strtok((char *)line, "\n");
        size_t ll = strlen(l);
        int i, h = 0;

        for (i = 0; i < ll; i++) {
            struct chrinfo *p = getchrinfo(l[i]);
            if (h < p->h) h = p->h;
        } /* for */
            
		if (lineno++) puts("");

        for (i = 0; i < h; i++) {
            int j;
            for (j = 0; j < ll; j++) {
                struct chrinfo *p = getchrinfo(l[j]);
                int pre1 = j 
                        ? 1
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
            puts("");
        } /* for */
    } /* for */
} /* process */
