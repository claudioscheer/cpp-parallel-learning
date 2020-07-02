FROM ubuntu:latest

COPY ./scripts/install-dependencies.sh /home
RUN sh /home/install-dependencies.sh
RUN rm /home/install-dependencies.sh