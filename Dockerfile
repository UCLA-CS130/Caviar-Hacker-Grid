FROM ubuntu:14.04

MAINTAINER Richard Min <richardmin97@gmail.com>

RUN apt-get update
RUN apt-get install -y libboost-all-dev make g++ 
RUN sudo apt-get install -y python-setuptools python-dev
RUN sudo easy_install pip
RUN sudo pip install boto3

WORKDIR /opt/webserver
COPY . /opt/webserver

COPY ./.aws/credentials $HOME/.aws/credentials

RUN make clean && make webserver

CMD tar -cf - webserver

