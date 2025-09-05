FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
        build-essential \
        ncurses-dev \
        vim \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /project

CMD [ "bash" ]