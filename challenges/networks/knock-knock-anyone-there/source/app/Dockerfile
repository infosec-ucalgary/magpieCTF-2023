FROM php:apache

RUN apt update && apt -y install tcpdump bash ncat libcap2-bin
RUN apt remove curl -y

## website files
COPY ./public /var/www/html/

RUN rm -rf /bin/ps
## add user 
ARG USER=sappheiros
RUN useradd -m ${USER} -s /bin/bash
RUN echo "${USER}:5up32_53cu23_p455w02d123" | chpasswd

## give user permission to run tcpdump
RUN groupadd pcap
RUN usermod -a -G pcap $USER
RUN chgrp pcap /usr/bin/tcpdump
RUN chmod -R 774 /usr/bin/tcpdump
RUN setcap cap_net_raw,cap_net_admin=eip /usr/bin/tcpdump

## edit some file permissions
RUN chmod -R 750 /tmp/
RUN chown -R root:root /var/www/html
RUN chmod -R 755 /var/www/html

## first part of flag
RUN chown -R root:${USER} /home/${USER}
RUN echo "part 1: magpie{y0u_h4v3_7h3_" >> /home/${USER}/message.txt && \
    chmod 440 /home/${USER}/message.txt
RUN chgrp ${USER} /home/${USER}/message.txt

## extra files one of which contains user password
RUN mkdir /opt/backup
RUN echo "5up32_53cu23_p455w02d123" >> /opt/backup/cGFzc3dvcmQ
RUN echo "try again"  >>  /opt/backup/aG90cm90dXJu
RUN echo "try again"  >>  /opt/backup/cGVscnVzdGVh
RUN echo "try again"  >>  /opt/backup/dGhhdnVuYQ
RUN echo "try again"  >>  /opt/backup/eHV6b3J0aA
RUN echo "try again"  >>  /opt/backup/a2VyaWxpYQ
RUN echo "try again"  >>  /opt/backup/cGVvcnVz
RUN echo "try again"  >>  /opt/backup/Ymlrb2xhcmE
RUN echo "try again"  >>  /opt/backup/bWl4aXJ1cw
RUN echo "try again"  >>  /opt/backup/cmlnbm9yaWE
RUN echo "try again"  >>  /opt/backup/a2l2YWVtaWE
RUN echo "try again"  >>  /opt/backup/bW9zeXBzbw
RUN echo "try again"  >>  /opt/backup/Y2Vsdm9yaXg
RUN echo "try again"  >>  /opt/backup/aWFwaHVz
RUN echo "try again"  >>  /opt/backup/eWl2aXM
RUN echo "try again"  >>  /opt/backup/c3RyZW5ldHVuZQ
RUN echo "try again"  >>  /opt/backup/c3RyaW1vc3RlYQ
RUN chmod -R 555 /opt/backup

USER www-data

EXPOSE 80