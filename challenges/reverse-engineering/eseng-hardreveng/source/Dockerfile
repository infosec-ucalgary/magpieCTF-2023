FROM alpine:latest

ARG UNAME=app
ARG PORT=31185

RUN apk add --update \
    fakeroot \
    libc-dev \
    gcc \
    make \
    sudo \
    socat \
    bash

RUN mkdir -p /jail/bin /jail/lib /home/$UNAME /jail/home /jail/etc; \
    touch /jail/etc/passwd; \
    cp /lib/ld-musl-x86_64.so.1 /jail/lib/; \
    cp /usr/lib/libreadline.so.8 /jail/lib; \
    cp /usr/lib/libncursesw.so.6 /jail/lib; \
    cp `which ash` /jail/bin/; \
    cp `which bash` /jail/bin/; \
    cp `which ls` /jail/bin; \
    cp `which cat` /jail/bin; \
    cp `which adduser` /jail/bin; \
    cp `which rm` /jail/bin; \
    cp `which su` /jail/bin; \
    adduser $UNAME -s /bin/bash -h /home/$UNAME; \
    echo "$UNAME ALL=NOPASSWD: /usr/sbin/chroot" >> /etc/sudoers

RUN echo "sudo chroot /jail/ /bin/bash -c 'source .bashrc'" > /home/$UNAME/.bashrc; \
    echo "adduser -D user; rm /bin/adduser`; su user;" > /jail/.bashrc;

COPY . /src/
COPY flag.txt /jail/

RUN chown -R root:root /jail/flag.txt; \
    cd /src/; \
    mkdir -p /src/build/; \
    make; \
    chown -R $UNAME:$UNAME /src; \
    mv /src/build/ /; \
    rm -rf /src;

USER $UNAME
ENV LD_LIBRARY_PATH=/build/
ENV SYS_PORT=$PORT

EXPOSE $PORT
CMD ["/bin/sh", "-c", "socat -d -d TCP-L:$SYS_PORT,fork EXEC:/build/shift_satellite,pty,stderr,rawer"]
