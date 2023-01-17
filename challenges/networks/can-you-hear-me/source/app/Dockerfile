FROM php:apache

RUN apt update && apt -y install tcpdump bash ncat libcap2-bin
RUN apt uninstall curl

## website files
COPY ./public /var/www/html/

## add user 
ARG USER=sappheiros
RUN useradd -m ${USER} -s /bin/bash
RUN echo "${USER}:5up32_53cu23_p455w02d123" | chpasswd

## give user permission to run tcpdump
RUN groupadd pcap
RUN usermod -a -G pcap $USER
RUN chgrp pcap /usr/bin/tcpdump
RUN setcap cap_net_raw,cap_net_admin=eip /usr/bin/tcpdump

## edit some file permissions
RUN chmod -R 750 /tmp/
RUN chown -R root:www-data /var/www/html
RUN chmod -R 740 /var/
RUN chmod -R 744 /etc/apache2/

## first part of flag
RUN chown -R root:${USER} /home/${USER}
RUN echo "part 1: magpie{y0u_h4v3_7h3_" >> /home/${USER}/message.txt && \
    chmod 440 /home/${USER}/message.txt
RUN chgrp ${USER} /home/${USER}/message.txt

## extra files one of which contains user password
RUN mkdir /opt/backup


USER www-data

EXPOSE 80