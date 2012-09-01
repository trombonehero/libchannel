/*! @file uds.c  UNIX domain socket implementation. */
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


static bool
valid_uds(uds_channel *c)
{
	if (c == NULL) return false;
	if (!channel_isvalid(&c->super)) return false;
	if (c->super.magic != UDS_MAGIC) return false;
	if (c->socket < 0) return false;

	return true;
}

uds_channel*
uds_create(int sock)
{
	uds_channel *channel = malloc(sizeof(uds_channel));
	channel_init(&channel->super, UDS_MAGIC, uds_size, uds_copy, uds_send);

	channel->socket = sock;

	assert(valid_uds(channel));

	return channel;
}

channel*
uds_wrap(uds_channel *c)
{
	assert(valid_uds(c));
	return (channel*) c;
}

uds_channel*
uds_unwrap(channel *c)
{
	assert(channel_isvalid(c));
	uds_channel *uds = (uds_channel*) c;
	assert(valid_uds(uds));

	return uds;
}


// channel implementation:

int
uds_size(__unused channel *c)
{
	return sizeof(uds_channel);
}

int
uds_copy(channel *c, void *dest)
{
	return -1;
}

int
uds_send(channel *c, message *m)
{
	return -1;
}

