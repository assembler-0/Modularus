/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once

#include <Types.h>

/* Custom ctype implementation for kernel environment */

#define _CTYPE_CNTRL  0x01
#define _CTYPE_SPACE  0x02
#define _CTYPE_PUNCT  0x04
#define _CTYPE_DIGIT  0x08
#define _CTYPE_XDIGIT 0x10
#define _CTYPE_UPPER  0x20
#define _CTYPE_LOWER  0x40
#define _CTYPE_GRAPH  0x80

/* Character classification functions */
static inline int isdigit(int c) {
    return (c >= '0' && c <= '9');
}

static inline int isxdigit(int c) {
    return (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

static inline int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

static inline int isupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

static inline int islower(int c) {
    return (c >= 'a' && c <= 'z');
}

static inline int isalpha(int c) {
    return isupper(c) || islower(c);
}

static inline int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

static inline int isgraph(int c) {
    return (c > 32 && c < 127);
}

static inline int isprint(int c) {
    return (c >= 32 && c < 127);
}

static inline int ispunct(int c) {
    return (c != ' ' && c != '\t' && c != '\n' && c != '\v' && c != '\f' && c != '\r' && isgraph(c) && !isalnum(c));
}

static inline int iscntrl(int c) {
    return ((c >= 0 && c < 32) || c == 127);
}

/* Character conversion functions */
static inline int tolower(int c) {
    return isupper(c) ? c - 'A' + 'a' : c;
}

static inline int toupper(int c) {
    return islower(c) ? c - 'a' + 'A' : c;
}