FROM ubuntu:bionic

RUN apt update && apt upgrade -y

RUN apt install python python-dev python-pip gcc make gdb -y

RUN pip install pwntools

WORKDIR /app

COPY cryptme.c Makefile encrypt_elf.py /app/

CMD ["/bin/bash"]