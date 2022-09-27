FROM ubuntu:20.04
MAINTAINER JasonLu "jason.lu@xcalibyte.com"
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN apt-get clean
RUN apt-get update && apt-get install -y vim gcc g++ make cmake emacs curl sudo python3

