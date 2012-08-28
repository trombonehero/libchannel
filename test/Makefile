PROG=	test-libchannel
INTERNALPROG=	yes
SRCS=	\
	ctest.c \
	main.c \
	fd.c \
	lifecycle.c \
	sendrecv.c \

NO_MAN=

CSTD?=	c99

INCDIR=		${.CURDIR}/../include

# We need -rdynamic to be able to do RTLD introspection.
CFLAGS+=	-Wall -Werror -I${INCDIR} -rdynamic
LDFLAGS+=	-L../libchannel -lchannel

all: ${PROG} run_tests
run_tests:
	sh ${.CURDIR}/test.t || exit 1

.include <bsd.prog.mk>
