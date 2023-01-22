FROM python:3

RUN apt update && apt -y install nmap 
RUN apt -y install procps

WORKDIR /pinger

COPY pinger.py pinger.py

CMD ["python3", "pinger.py"]
