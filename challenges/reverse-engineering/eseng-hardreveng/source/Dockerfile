FROM alpine:latest

ARG UNAME=app
ARG PORT=31185

RUN apk add --update \
    fakeroot \
    libc-dev \
    gcc \
    make \
    sudo \
    socat

RUN mkdir -p /jail/bin /jail/lib /jail/home/$UNAME; \
    cp /lib/ld-musl-x86_64.so.1 /jail/lib/; \
    cp `which ash` /jail/bin/; \
    ln -s /jail/bin/ash /jail/bin/bash; \
    cp `which ls` /jail/bin; \
    cp `which cat` /jail/bin; \
    adduser $UNAME -s /bin/ash -h /jail/home/$UNAME; \
    echo "$UNAME ALL=NOPASSWD: /usr/sbin/chroot" >> /etc/sudoers

COPY . /src/

RUN cd /src/; \
    mkdir -p /src/build/; \
    make; \
    chown -R $UNAME:$UNAME /src; \
    mv /src/build/ /;

USER $UNAME
ENV LD_LIBRARY_PATH=/build/
ENV SYS_PORT=$PORT

EXPOSE $PORT
CMD ["/bin/sh", "-c", "socat -d -d TCP-L:$SYS_PORT,fork EXEC:/build/shift_satellite,pty,stderr,rawer"]
