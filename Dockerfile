FROM ubuntu:latest

ARG DEBIAN_FRONTED=noninteractive
ENV TZ=Europe/Moscow

RUN apt-get update \
    && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
    && apt install -y python3.8 \
    && apt install -y python3-pip \
    && apt install -y build-essential \
    && apt-get install -y valgrind \
    && apt-get install -y gdb \
    && apt install -y tmux