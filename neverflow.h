/*
 * NEVERFLOW - v0.0.2 - Set of macros that guard against buffer overflows. 
 * Based on C99 VLA feature.
 * 
 * MIT LICENSE
 *
 * Copyright <2023> <skullchap> <https://github.com/skullchap/>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Changes
 *      0.0.2 - name mangling removed, 
 *              added ARR() macro to ease passing arrays to functions.
 *              Another neat sideeffect of it, is possibility of wrapping raw pointer 
 *              and providing runtime bound checking safety.            
 *      0.0.1 - initial release
 * Contributors <>
 *
 */

#ifndef NEVERFLOW_H_
#define NEVERFLOW_H_

#define VERSION(MAJOR, MINOR, PATCH) (((MAJOR) << 22) | ((MINOR) << 12) | (PATCH))
static const int NEVERFLOW_VERSION = VERSION(0, 0, 2);

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char uchar;
#ifndef ALLOCF
    #define __calloc(nbytes) calloc(nbytes,1)
    #define ALLOCF __calloc
#endif
#define ERRLOG(fmt, ...) fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define ASSERT(expr, fmt, ...)                     \
    (!(expr)) ? (ERRLOG(fmt, ##__VA_ARGS__),       \
                 ERRLOG("Function: %s", __func__), \
                 abort())                          \
              : ((void)0)
#ifndef NO_AUTOFREE
    __attribute__((always_inline)) inline void __free(void *ptr){free(*(void **)ptr);}
    #define autofree __attribute__((cleanup(__free)))
#else
    #define autofree
#endif
#define NEW(TYPE, NAME, COUNT) autofree TYPE(*NAME)[COUNT] = ALLOCF(sizeof *NAME)
#define AT(NAME, IDX)                                                       \
    ((typeof(&(*NAME)[0]))                                            \
    ((ASSERT(((size_t)IDX) * sizeof(*NAME)[0] < sizeof *NAME,   \
    "Buffer Overflow. Index [%lu] is out of range [0-%lu]",                 \
    ((size_t)IDX), ((sizeof *NAME / sizeof(*NAME)[0]) - 1))),   \
    ((uchar *)*NAME) + ((size_t)IDX) * sizeof(*NAME)[0]))
#define GET(NAME, IDX) *AT(NAME, IDX)
#define LET __auto_type
#define SIZE(NAME) (sizeof *NAME)
#define LEN(NAME) (sizeof *NAME / sizeof(*NAME)[0])
#define ARR(TYPE, NAME, COUNT) TYPE(*NAME)[COUNT]

#endif
