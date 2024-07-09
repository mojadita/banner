/* char_table.h -- types and definitions to use a char table.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Fri Dec 22 10:40:46 EET 2023
 * Copyright: 2023 Luis Colorado.  All rights reserved.
 * License: BSD.
 */
#ifndef CHAR_TABLE_H
#define CHAR_TABLE_H

#include<wchar.h>
#include<sys/types.h>

struct chrinfo {
    size_t w,   /* width  of char */
		   h;   /* height of char */
    char **s;   /* strings array */
};

struct range {
    wchar_t fst;
    wchar_t lst;
    struct chrinfo *ci;
};

size_t chartable_init(void);
struct chrinfo *getchrinfo(wchar_t c);

#endif /* CHAR_TABLE_H */
