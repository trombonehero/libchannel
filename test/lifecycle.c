/*-
 * Copyright (c) 2012 Jonathan Anderson
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <libchannel.h>

#include "ctest.h"

int
test_lifecycle()
{
	int success = PASSED;

	// Non-channels should be recognized as such.
	struct channel *garbage = NULL;
	CHECK(!channel_isvalid(garbage));
	CHECK(channel_flags(garbage) == -1);

	int some_integer;
	garbage = (struct channel*) &some_integer;
	CHECK(!channel_isvalid(garbage));

	// Make sure we can create, recognize and destroy channels.
	int flags_to_test[] = {
	   0,
	   CHANNEL_RELIABLE,
	   CHANNEL_INORDER,
	   CHANNEL_RELIABLE | CHANNEL_INORDER
	};

	for (size_t i = 0; i < sizeof(flags_to_test) / sizeof(int); i++) {
		int flags = flags_to_test[i];

		struct channel *c = channel_create(flags);
		CHECK(c != NULL);
		CHECK(channel_isvalid(c));
		CHECK(channel_flags(c) == flags);
		channel_destroy(c);
	}

	return success;
}

