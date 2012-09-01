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
#include <string.h>


void
channel_init(channel *c, int magic,
             int (*size)(channel*),
             int (*copy)(channel*, void *dest),
             int (*send)(channel*, struct message*)
            )
{
	assert(c != NULL);

	c->magic = magic;

	c->size = size;
	c->copy = copy;
	c->send = send;
}

int
channel_size(channel *c)
{
	assert(channel_isvalid(c));

	int size = c->size(c);
	assert(size > 0);

	return size;
}

int
channel_copy(void *dest, channel *src)
{
	assert(channel_isvalid(src));

	int size = channel_size(src);
	memcpy(dest, src, size);

	return size;
}

channel*
channel_wrap_socket(int sock)
{
	return uds_wrap(uds_create(sock));
}

bool
channel_isvalid(channel *c)
{
	if (c == NULL) return false;
	if (c->size == NULL) return false;
	if (c->copy == NULL) return false;
	if (c->send == NULL) return false;

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
channel_send(struct channel *c, struct message *m)
{
	if (!channel_isvalid(c)) return -1;
	return c->send(c, m);
}

