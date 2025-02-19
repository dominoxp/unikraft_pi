FROM ubuntu:24.04

RUN wget -O install.sh https://get.kraftkit.sh
RUN chmod +x install.sh && ./install.sh -y

COPY . /data

ENTRYPOINT ["/data/build.sh"]
