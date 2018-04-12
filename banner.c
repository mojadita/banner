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

#ifndef UTF
#define UTF 0
#endif

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
    "  ????",
    " ?    ?",
    "?      ?",
    "?      ?",
    "????????",
    "  ?  ?",
    "???  ???",
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
    "#     #",
    "#  #  #",
    "#     #",
    " #   #",
    "  ###",
#if 0
    "  ###",
    " #   #",
    "#   # #",
    "#  #  #",
    "# #   #",
    " #   #",
    "  ###",
#endif
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
    "# #### ",
    "##   ##",
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

static char *c_iexclam[] = {
	"#",
	"",
	"#",
	"#",
	"#",
	"#",
	"#",
	"#",
	"#",
	0
};

static char *c_cent[] = {
    "",
    "   #",
    " #####",
    "#  #  #",
    "#  #",
    "#  #",
    " #####",
	"   #",
    0
};

static char *c_pound[] = {
	"   ####",
	"  #    #",
	"  #",
	" #####",
	"  #",
	"  #    #",
	"########",
	0,
};

static char *c_currency[] = {
    "# ### #",
    " #   #",
    "#     #",
    "#     #",
    "#     #",
    " #   #",
    "# ### #",
    0
};

static char *c_yen[] = {
	"#     #",
	" #   #",
	"#######",
	"   #",
	"#######",
	"   #",
	"   #",
	0
};

static char *c_bbar[] = {
	"#",
	"#",
	"#",
	"#",
	"",
	"#",
	"#",
	"#",
	"#",
	0
};

static char *c_section[] = {
    " #####",
    "#",
    "# ###",
    " #   #",
    "  ### #",
    "      #",
    " #####",
    0
};

static char *c_diaeresis[] = {
	"##   ##",
	"##   ##",
	0
};

static char *c_copyright[] = {
    "  ###  ",
    " #   #",
    "#  ## #",
    "# #   #",
    "#  ## #",
    " #   #",
    "  ###  ",
    0
};

static char *c_femord[] = {
    " ####",
    "#   #",
    " ### #",
	"",
	"######",
    0
};

static char *c_lpdaq[] = {
	"",
	"",
    "  # #",
    " # #",
    "# #",
    " # #",
    "  # #",
    0
};

static char *c_not[] = {
	"",
	"",
	"",
	"",
	"#######",
	"      #",
	"      #",
	0
};

static char *c_softhyp[] = {
	"",
	"",
	"",
	"#     #",
	"#######",
	"#     #",
	0
};


static char *c_reg[] = {
    "  ###  ",
    " #   #",
    "# ### #",
    "# #  ##",
    "# ### #",
    " ## ##",
    "  ###  ",
    0
};

static char *c_macron[] = {
	"#######",
	0
};

static char *c_degree[] = {
	" ###",
	"#   #",
	" ###",
	0
};

static char *c_plusminus[] = {
    "",
    "",
    "   #",
    "   #",
    "#######",
    "   # ",
    "#######",
    0
};

static char *c_sup2[] = {
    " ####",
    "#    #",
    "   ##",
    " ##",
    "######",
    0
};

static char *c_sup3[] = {
    " ####",
    "#    #",
    "   ##",
    "#    #",
    " ####",
    0
};

static char *c_acute[] = {
    "  ##",
    " ##",
    "#",
    0
};

static char *c_micro[] = {
    "",
    "",
    "#    #",
    "#    #",
    "#    #",
    "#    #",
    "##### #",
    "#",
    "#",
    0
};

static char *c_pilcrow[] = {
    " #######",
    "##### #",
    "##### #",
    " ######",
    "    # #",
    "    # #",
    "    # #",
    0
};

static char *c_cdot[] = {
    "",
    "",
    "",
    "##",
    "##",
    0
};

static char *c_cedilla[] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "   #",
	"   #",
	"  #",
    0
};

static char *c_sup1[] = {
    "  #  ",
    " ##",
    "  #",
    "  #",
    " ###",
    0
};

static char *c_mascord[] = {
    " ####",
    "#    #",
    " ####",
	"",
	"######",
    0
};

static char *c_rpdaq[] = {
	"",
	"",
    "# #",
    " # #",
    "  # #",
    " # #",
    "# #",
    0
};

static char *c_1_4[] = {
    " #    #",
    "##   #",
    " #  #",
    " # # ##",
    "### # #",
    " # #####",
    "#     #",
    0
};

static char *c_1_2[] = {
    " #    #",
    "##   #",
    " #  #",
    " # # ##",
    "### #  #",
    " #    #",
    "#   ####",
    0
};

static char *c_3_4[] = {
    "###   #",
    " #   #",
    "  # #",
    "## # ##",
    "  # # #",
    " # #####",
    "#     #",
    0
};

static char *c_lqm[] = {
    "",
    "",
    "   #",
    "",
    "   #",
    " ##",
    "#",
    "#     #",
    " #####",
    0
};

static char *c_Agrave[] = {
	"  #",
	"   #",
	"",
    "  ###",
    "#     #",
    "#######",
    "#     #",
    0
};

static char *c_Aacute[] = {
	"    #",
	"   #",
	"",
    "  ###",
    "#     #",
    "#######",
    "#     #",
    0
};

static char *c_Acirc[] = {
	"   #",
	"  # #",
	"",
    "  ###",
    "#     #",
    "#######",
    "#     #",
    0
};

static char *c_Atilde[] = {
	"  #  #",
	" # ##",
	"",
    "  ###",
    "#     #",
    "#######",
    "#     #",
    0
};

static char *c_Adiaer[] = {
	" #   # ",
	"",
    "  ###",
    "#     #",
    "#     #",
    "#######",
    "#     #",
    0
};

static char *c_Aring[] = {
	"   #",
	"  # #",
	"   #",
    "  ###",
    "#     #",
    "#######",
    "#     #",
    0
};

static char *c_AE[] = {
    "   #####",
    "  ##",
    " # #",
    "#  ####",
    "####",
    "#  #",
    "#  #####",
    0
};

static char *c_Ccedil[] = {
    " #####",
    "#     #",
    "#",
    "#",
    "#",
    "#     #",
    " #####",
	"   #",
	"  #",
    0
};

static char *c_Egrave[] = {
	"  #",
	"   #",
    "#######",
    "#",
    "####",
    "#",
    "#######",
    0
};

static char *c_Eacute[] = {
	"    #",
	"   #",
	"#######",
    "#",
    "####",
    "#",
    "#######",
    0
};

static char *c_Ecirc[] = {
	"   #",
	"  # #",
	"#######",
    "#",
    "####",
    "#",
    "#######",
    0
};

static char *c_Ediaer[] = {
	" #   # ",
	"",
	"#######",
    "#",
    "####",
    "#",
    "#######",
    0
};

static char *c_Igrave[] = {
	"#",
	" #",
    "###",
    " #",
    " #",
    " #",
    "###",
    0
};

static char *c_Iacute[] = {
	"  #",
	" #",
    "###",
    " #",
    " #",
    " #",
    "###",
    0
};

static char *c_Icirc[] = {
	" #",
	"# #",
	"",
    "###",
    " #",
    " #",
    "###",
    0
};

static char *c_Idiaer[] = {
	"#   #",
	"",
	" ###",
    "  #",
    "  #",
    "  #",
    " ###",
    0
};

static char *c_Eth[] = {
    "#####",
    " #   ##",
    " #     #",
    "####   #",
    " #     #",
    " #   ##",
    "#####",
    0
};

static char *c_Ntilde[] = {
	"  #  #",
	" # ##",
	"",
    "#     #",
    "# #   #",
    "#   # #",
    "#     #",
    0
};

static char *c_Ograve[] = {
	"  #",
	"   #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Oacute[] = {
	"    #",
	"   #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Ocirc[] = {
	"   #",
	"  # #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Otilde[] = {
	"  #  #",
	" # ##",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Odiaer[] = {
	" #   #",
	"",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_multSign[] = {
    "",
    "",
    "#       #",
    "  #   #",
    "    #",
    "  #   #",
    "#       #",
    0
};

static char *c_Ostroke[] = {
    " #### #",
    "#    # ",
    "#   # #",
    "#  #  #",
    "# #   #",
    " #    #",
    "# ####",
    0
};

static char *c_Ugrave[] = {
	"  #",
	"   #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Uacute[] = {
	"    #",
	"   #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Ucirc[] = {
	"   #",
	"  # #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Udiaer[] = {
	" #   #",
	"",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_Yacute[] = {
	"  #",
	"   #",
    "#     #",
    "  # #",
    "   #",
    "   #",
    0
};

static char *c_Thorn[] = {
    "#",
    "#",
    "######",
    "#     #",
    "#     #",
    "######",
    "#",
    "#",
    0
};

static char *c_ssharp[] = {
	"  ###",
	" #   #",
	"#    #",
	"#####",
	"#    #",
	"#     #",
	"#     #",
	"######",
	0
};

static char *c_agrave[] = {
    "  #",
    "   #",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_aacute[] = {
    "    #",
    "   #",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_acirc[] = {
    "   #",
    "  # #",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_atilde[] = {
    "  ### #",
    " #   #",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_adiaer[] = {
    " #    #",
    "",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_aring[] = {
    "  ###",
    "  # #",
    " #####",
    "      #",
    " ######",
    "#     #",
    " ##### #",
    0
};

static char *c_ae[] = {
    "",
    "",
    " ### ##",
    "    #  #",
    " ######",
    "#   #",
    " ### ##",
    0
};

static char *c_ccedil[] = {
    "",
    "",
    " #####",
    "#     #",
    "#",
    "#",
    " #####",
	"   #",
	"  #",
    0
};

static char *c_egrave[] = {
    "  #",
    "   #",
    " #####",
    "#     #",
    "######",
    "#",
    " ######",
    0
};

static char *c_eacute[] = {
    "    #",
    "   #",
    " #####",
    "#     #",
    "######",
    "#",
    " ######",
    0
};

static char *c_ecirc[] = {
    "   #",
    "  # #",
    " #####",
    "#     #",
    "######",
    "#",
    " ######",
    0
};

static char *c_ediaer[] = {
    " #   #",
    "",
    " #####",
    "#     #",
    "######",
    "#",
    " ######",
    0
};

static char *c_igrave[] = {
    "#",
    " #",
    "",
    " #",
    " #",
    " #",
    "  ##",
    0
};

static char *c_iacute[] = {
    " #",
    "#",
    "",
    "#",
    "#",
    "#",
    " ##",
    0
};

static char *c_icirc[] = {
    " #",
    "# #",
    "",
    " #",
    " #",
    " #",
    "  ##",
    0
};

static char *c_idiaer[] = {
    "#   #",
    "",
    "  #",
    "  #",
    "  #",
    "  #",
    "   ##",
    0
};

static char *c_eth[] = {
	" #  #",
	"  #",
	"#  #",
	" ####",
	"#    #",
	"#    #",
	" ####",
	0
};

static char *c_ntilde[] = {
    "#   ##",
    " ###  #",
    "",
    "# ####",
    "##    #",
    "#     #",
    "#     #",
    0
};

static char *c_ograve[] = {
    "  #",
    "   #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_oacute[] = {
    "    #",
    "   #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_ocirc[] = {
    "   #",
    "  # #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_otilde[] = {
    "#  ###",
    " ##   #",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_odiaer[] = {
    " #   #",
    "",
    " #####",
    "#     #",
    "#     #",
    "#     #",
    " #####",
    0
};

static char *c_divsign[] = {
    "",
    "",
    "   ##",
    "",
    "########",
    "",
    "   ##",
    0
};

static char *c_ostroke[] = {
    "",
    "",
    " #####",
    "#    ##",
    "#  #  #",
    "##    #",
    " #####",
    0
};

static char *c_ugrave[] = {
    "  #",
    "   #",
    "#    #",
    "#    #",
    "#    #",
    "#    #",
    " #### #",
    0
};

static char *c_uacute[] = {
    "   #",
    "  #",
    "#    #",
    "#    #",
    "#    #",
    "#    #",
    " #### #",
    0
};

static char *c_ucirc[] = {
    "   #",
    "  # #",
    "#    #",
    "#    #",
    "#    #",
    "#    #",
    " #### #",
    0
};

static char *c_udiaer[] = {
    "#    #",
    "",
    "#    #",
    "#    #",
    "#    #",
    "#    #",
    " #### #",
    0
};

static char *c_yacute[] = {
    "    #",
    "   #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " ######",
    "      #",
    " #####",
    0
};

static char *c_thorn[] = {
    "",
    "",
    "#",
    "######",
    "#     #",
    "#     #",
    "######",
    "#",
    0
};

static char *c_ydiaer[] = {
    "#     #",
    "",
    "",
    "#     #",
    "#     #",
    "#     #",
    " ######",
    "      #",
    " #####",
    0
};

static struct chrinfo  ci_invalid[] = {
	{0, 0, c_unknown,},
};

static struct chrinfo  latin1_0[] = {
    /* 0x20 */ {7,0,c_space},
	/* 0x21 */ {0,0,c_exclam}, {0,0,c_dquot}, {0,0,c_hash},
	/* 0x24 */ {0,0,c_dollar}, {0,0,c_percent}, {0,0,c_ampers},
	/* 0x27 */ {0,0,c_quote}, {0,0,c_lparen}, {0,0,c_rparen},
	/* 0x2a */ {0,0,c_asterisk}, {0,0,c_plus}, {0,0,c_comma},
	/* 0x2d */ {0,0,c_minus}, {0,0,c_dot}, {0,0,c_slash},
	/* 0x30 */ {0,0,c_0}, {0,0,c_1}, {0,0,c_2}, {0,0,c_3},
    /* 0x34 */ {0,0,c_4}, {0,0,c_5}, {0,0,c_6}, {0,0,c_7},
	/* 0x38 */ {0,0,c_8}, {0,0,c_9}, {0,0,c_colon},
	/* 0x3b */ {0,0,c_semicolon}, {0,0,c_less}, {0,0,c_eq},
	/* 0x3e */ {0,0,c_greater}, {0,0,c_qmark}, {0,0,c_atsign},
	/* 0x41 */ {0,0,c_A}, {0,0,c_B}, {0,0,c_C}, {0,0,c_D},
	/* 0x45 */ {0,0,c_E}, {0,0,c_F}, {0,0,c_G}, {0,0,c_H},
	/* 0x49 */ {0,0,c_I}, {0,0,c_J}, {0,0,c_K}, {0,0,c_L},
	/* 0x4d */ {0,0,c_M}, {0,0,c_N}, {0,0,c_O}, {0,0,c_P},
	/* 0x51 */ {0,0,c_Q}, {0,0,c_R}, {0,0,c_S}, {0,0,c_T},
	/* 0x55 */ {0,0,c_U}, {0,0,c_V}, {0,0,c_W}, {0,0,c_X},
	/* 0x59 */ {0,0,c_Y}, {0,0,c_Z}, {0,0,c_lbkt},
	/* 0x5c */ {0,0,c_bslsh}, {0,0,c_rbkt}, {0,0,c_circunf},
	/* 0x5f */ {0,0,c_undscore}, {0,0,c_lquot}, {0,0,c_a},
	/* 0x62 */ {0,0,c_b}, {0,0,c_c}, {0,0,c_d}, {0,0,c_e},
	/* 0x66 */ {0,0,c_f}, {0,0,c_g}, {0,0,c_h}, {0,0,c_i},
	/* 0x6a */ {0,0,c_j}, {0,0,c_k}, {0,0,c_l}, {0,0,c_m},
	/* 0x6e */ {0,0,c_n}, {0,0,c_o}, {0,0,c_p}, {0,0,c_q},
	/* 0x72 */ {0,0,c_r}, {0,0,c_s}, {0,0,c_t}, {0,0,c_u},
	/* 0x76 */ {0,0,c_v}, {0,0,c_w}, {0,0,c_x}, {0,0,c_y},
	/* 0x7a */ {0,0,c_z}, {0,0,c_lcbkt}, {0,0,c_vbar},
	/* 0x7d */ {0,0,c_rcbkt}, {0,0,c_tilde},
};

static struct chrinfo latin1_1[] = {
	/* 0xA0 */ {7,0,c_space}, {0,0,c_iexclam}, {0,0,c_cent}, {0,0,c_pound},
	/* 0xA4 */ {0,0,c_currency},{0,0,c_yen},{0,0,c_bbar},{0,0,c_section},
	/* 0xA8 */ {0,0,c_diaeresis},{0,0,c_copyright},{0,0,c_femord},{0,0,c_lpdaq},
	/* 0xAC */ {0,0,c_not},{7,0,c_softhyp},{0,0,c_reg},{0,0,c_macron},
	/* 0xB0 */ {0,0,c_degree},{0,0,c_plusminus},{0,0,c_sup2},{0,0,c_sup3},
	/* 0xB4 */ {0,0,c_acute},{0,0,c_micro},{0,0,c_pilcrow},{0,0,c_cdot},
	/* 0xB8 */ {0,0,c_cedilla},{0,0,c_sup1},{0,0,c_mascord},{0,0,c_rpdaq},
	/* 0xBC */ {0,0,c_1_4},{0,0,c_1_2},{0,0,c_3_4},{0,0,c_lqm},
	/* 0xC0 */ {0,0,c_Agrave},{0,0,c_Aacute},{0,0,c_Acirc},{0,0,c_Atilde},
	/* 0xC4 */ {0,0,c_Adiaer},{0,0,c_Aring},{0,0,c_AE},{0,0,c_Ccedil},
	/* 0xC8 */ {0,0,c_Egrave},{0,0,c_Eacute},{0,0,c_Ecirc},{0,0,c_Ediaer},
	/* 0xCC */ {0,0,c_Igrave},{0,0,c_Iacute},{0,0,c_Icirc},{0,0,c_Idiaer},
	/* 0xD0 */ {0,0,c_Eth},{0,0,c_Ntilde},{0,0,c_Ograve},{0,0,c_Oacute},
	/* 0xD4 */ {0,0,c_Ocirc},{0,0,c_Otilde},{0,0,c_Odiaer},{0,0,c_multSign},
	/* 0xD8 */ {0,0,c_Ostroke},{0,0,c_Ugrave},{0,0,c_Uacute},{0,0,c_Ucirc},
	/* 0xDC */ {0,0,c_Udiaer},{0,0,c_Yacute},{0,0,c_Thorn},{0,0,c_ssharp},
	/* 0xE0 */ {0,0,c_agrave},{0,0,c_aacute},{0,0,c_acirc},{0,0,c_atilde},
	/* 0xE4 */ {0,0,c_adiaer},{0,0,c_aring},{0,0,c_ae},{0,0,c_ccedil},
	/* 0xE8 */ {0,0,c_egrave},{0,0,c_eacute},{0,0,c_ecirc},{0,0,c_ediaer},
	/* 0xEC */ {0,0,c_igrave},{0,0,c_iacute},{0,0,c_icirc},{0,0,c_idiaer},
	/* 0xF0 */ {0,0,c_eth},{0,0,c_ntilde},{0,0,c_ograve},{0,0,c_oacute},
	/* 0xF4 */ {0,0,c_ocirc},{0,0,c_otilde},{0,0,c_odiaer},{0,0,c_divsign},
	/* 0xF8 */ {0,0,c_ostroke},{0,0,c_ugrave},{0,0,c_uacute},{0,0,c_ucirc},
	/* 0xFC */ {0,0,c_udiaer},{0,0,c_yacute},{0,0,c_thorn},{0,0,c_ydiaer},
};

struct range ranges[] = {
	{ 0xfffe, 0xffff, ci_invalid },
	{ ' ', 0x7f, latin1_0 },
	{ 0xa0, 0xff, latin1_1 },

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
#define FLAG_UTF	(1 << 2)
int max_width = 0;

int main(int argc, char **argv)
{
    int opt;

	setlocale(LC_ALL, "");

    while ((opt = getopt(argc, argv, "fmu")) != EOF) {
        switch(opt) {
        case 'f': flags |= FLAG_FRAME; break;
        case 'm': flags |= FLAG_MONOSP; break;
		case 'u': flags |= FLAG_UTF; break;
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
				hor_line(this_l,
					flags & FLAG_UTF
						? "\u2552\u2550"
						: ",=",
					flags & FLAG_UTF
						? "\u2550\u2555\n"
						: "=.\n");
            } else if (this_l == 0) {
				hor_line(last_l,
					flags & FLAG_UTF
						? "\u2558\u2550"
						: "`=",
					flags & FLAG_UTF
						? "\u2550\u255b\n"
						: "='\n");
            } else if (last_l == this_l) {
				hor_line(last_l,
					flags & FLAG_UTF
						? "\u255e\u2550"
						: ">=",
					flags & FLAG_UTF
						? "\u2550\u2561\n"
						: "=<\n");
            } else { /* last_l != this_l, both != 0 */
                size_t min = MIN(last_l, this_l);
                size_t max = MAX(last_l, this_l);
                hor_line(min + 1,
					flags & FLAG_UTF
						? "\u255e\u2550"
						: ">=",
					flags & FLAG_UTF
						? last_l > this_l
							? "\u2564"
							: "\u2567"
						: last_l > this_l
							? "V"
							: "^");
                hor_line(max - min - 1,
                    "",
					flags & FLAG_UTF
						? last_l > this_l
							? "\u255b\n"
							: "\u2555\n"
						: last_l > this_l
							? "'\n"
							: ".\n");
            } /* else */
        } else {
            if (lineno++) puts("");
        } /* if */

        for (i = 0; i < h; i++) {
            int j;
            if (flags & FLAG_FRAME && len)
				printf(flags & FLAG_UTF
					? "\u2502 "
					: "| ");
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
            puts(flags & FLAG_FRAME && len
				? flags & FLAG_UTF
					? " \u2502"
					: " |"
				: "");
        } /* for */
        last_l = this_l;
    } /* while */
    if (flags & FLAG_FRAME && last_l) {
        hor_line(this_l,
			flags & FLAG_UTF
				? "\u2558\u2550"
				: "`=",
			flags & FLAG_UTF
				? "\u2550\u255b\n"
				: "='\n");
    } /* if */
} /* process */

void hor_line(size_t len, const char *lft, const char *rgt)
{
	char *the_line = flags & FLAG_UTF
		?  "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
		   "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
		   "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
		   "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
		:  "============================================================";
	if (flags & FLAG_UTF) len *= 3;
    size_t the_line_size = strlen(the_line);
    printf("%s", lft);
    while (len > the_line_size) {
        printf("%s", the_line);
        len -= the_line_size;
    } /* while */
    /* len <= the_line_size */
    printf("%.*s%s", len, the_line, rgt);
} /* hor_line */
