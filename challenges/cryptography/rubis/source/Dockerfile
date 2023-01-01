
FROM alpine:3.14

RUN apk update
RUN apk add --update alpine-sdk
RUN apk add python3 socat bash
RUN apk add py3-pip
RUN pip install pycryptodome sympy

RUN adduser -D rubis
USER rubis
WORKDIR /home/rubis
COPY challenge.py .

EXPOSE 20000

CMD socat TCP-LISTEN:20000,fork,keepalive,keepidle=10,keepintvl=10,keepcnt=2,reuseaddr EXEC:"python3 /home/rubis/challenge.py"