FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates \
        build-essential \
        bc \
        flex \
        bison \
        python3 \
        libssl-dev \
        libelf-dev \
        libncurses-dev \
        git \
        cpio \
        file \
        patchelf \
        xorriso \
        mtools \
        dosfstools \
        qemu-efi-aarch64 \
        ovmf \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build/kernel
RUN git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git /build/kernel

WORKDIR /project

CMD [ "bash" ]