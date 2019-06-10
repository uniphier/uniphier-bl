/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __SPRINTF_H__
#define __SPRINTF_H__

#include <stdarg.h>
#include <types.h>

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
int snprintf(char *buf, size_t size, const char *fmt, ...);
int scnprintf(char *buf, size_t size, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

#endif /* __SPRINTF_H__ */
