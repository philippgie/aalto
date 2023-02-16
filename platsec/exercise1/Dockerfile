# Dockerfile

FROM ubuntu:latest

RUN apt update && apt install -y apt-utils curl build-essential

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

ENV PATH="/root/.cargo/bin:${PATH}"

WORKDIR /usr/src

COPY main.rs .

RUN rustc -o main main.rs

RUN mv main /usr/local/bin/main

CMD /usr/local/bin/main

