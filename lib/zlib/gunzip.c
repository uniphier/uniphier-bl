// SPDX-License-Identifier: GPL-2.0+
//
// Based on lib/decompress_inflate.c, Linux 4.9
// Modified by Masahiro Yamada <yamada.masahiro@socionext.com>

#include <errno.h>
#include <gunzip.h>
#include <malloc.h>
#include <printk.h>
#include <types.h>

#include "inflate.h"
#include "infutil.h"
#include "zconf.h"
#include "zlib.h"

int gunzip(const void *in_buf, unsigned long in_len, void *out_buf,
	   void **out_pos)
{
	const u8 *zbuf = in_buf;
	struct z_stream_s *strm;
	int zret, ret = 0;

	strm = malloc(sizeof(*strm));
	if (!strm)
		return -ENOMEM;

	strm->workspace = malloc(sizeof(struct inflate_state));
	if (!strm->workspace) {
		ret = -ENOMEM;
		goto free;
	}

	if (!in_len)
		in_len = ~0UL;

	/* verify the gzip header */
	if (in_len < 10 ||
	    zbuf[0] != 0x1f || zbuf[1] != 0x8b || zbuf[2] != 0x08) {
		pr_err("Not a gzip file\n");
		ret = -EIO;
		goto free;
	}

	/*
	 * skip over gzip header (1f,8b,08... 10 bytes total +
	 * possible asciz filename)
	 */
	strm->next_in = zbuf + 10;
	strm->avail_in = in_len - 10;
	/* skip over asciz filename */
	if (zbuf[3] & 0x8) {
		do {
			if (strm->avail_in == 0) {
				pr_err("header error\n");
				ret = -EIO;
				goto free;
			}
			--strm->avail_in;
		} while (*strm->next_in++);
	}

	strm->next_out = out_buf;
	strm->avail_out = ~0UL - (unsigned long)out_buf; /* no limit */

	zret = zlib_inflateInit2(strm, -MAX_WBITS);

	WS(strm)->inflate_state.wsize = 0;
	WS(strm)->inflate_state.window = NULL;

	while (zret == Z_OK) {
		if (strm->avail_in == 0) {
			pr_err("read error\n");
			ret = -EIO;
			goto end;
		}
		zret = zlib_inflate(strm, 0);
	}

	/* after Z_FINISH, only Z_STREAM_END is "we unpacked it all" */
	if (zret != Z_STREAM_END) {
		pr_err("uncompression error\n");
		ret = -EIO;
	}

end:
	zlib_inflateEnd(strm);
	if (out_pos)
		*out_pos = strm->next_out;
free:
	free(strm->workspace);
	free(strm);

	return ret;
}
