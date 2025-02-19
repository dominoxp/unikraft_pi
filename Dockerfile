FROM ubuntu:24.04

RUN wget -O install.sh https://get.kraftkit.sh
RUN chmod +x install.sh && ./install.sh -y

COPY . /data
WORKDIR /data

RUN kraft build --plat qemu --arch arm64 | tee build.log

ENTRYPOINT ["/data/build.sh"]
