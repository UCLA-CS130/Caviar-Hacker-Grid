FROM ubuntu:14.04

MAINTAINER Richard Min <richardmin97@gmail.com>

RUN apt-get update
RUN apt-get install -y libboost-all-dev make g++

WORKDIR /opt/webserver
COPY . /opt/webserver

RUN make clean && make webserver

CMD ["./webserver", "test_config"]

