/*! @file libchannel.c  Implementation of platform-portable parts. */
/*-
 * Copyright (c) 2012 Jonathan Anderson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "libchannel.h"
#include "libchannel-internal.h"

#include <assert.h>
#include <stdlib.h>


channel*
channel_create(int flags)
{
	// TODO: choose among UDS, something else?
	return uds_wrap(uds_create(flags));
}

bool
channel_isvalid(channel *c)
{
	if (c == NULL) return false;
	if (c->get_descriptor == NULL) return false;

	// As we implement other kinds of channels, add more magic tests here.
	if (c->magic == UDS_MAGIC) return true;

	return false;
}

void
channel_destroy(channel *c)
{
	assert(channel_isvalid(c));
	free(c);
}

int
channel_flags(channel *c)
{
	if (!channel_isvalid(c)) return -1;
	return c->flags;
}

int
channel_descriptor(channel *c)
{
	if (!channel_isvalid(c)) return -1;
	return c->get_descriptor(c);
}

