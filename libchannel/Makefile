LIB=	channel

INCDIR=	${.CURDIR}/../include
MANDIR=	${.CURDIR}/../man

INCS=	${INCDIR}/libchannel.h
SRCS=	\
	libchannel.c      \
	uds.c             \


CFLAGS+=	-Wall -I ${.CURDIR} -I ${INCDIR}

SHLIB_MAJOR=	1

MAN=	${MANDIR}/libchannel.3
MLINKS=	\
	libchannel.3	channel_create.3        \
	libchannel.3	channel_isvalid.3       \
	libchannel.3	channel_destroy.3       \
	libchannel.3	channel_flags.3         \
	libchannel.3	channel_descriptor.3    \

.include <bsd.lib.mk>

