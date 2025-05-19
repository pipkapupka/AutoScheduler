FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libcurl4-openssl-dev \
    libssl-dev \
    zlib1g-dev \
    git \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/yhirose/cpp-httplib && \
    mkdir -p /usr/local/include && \
    cp cpp-httplib/httplib.h /usr/local/include/

WORKDIR /app
COPY . .

RUN rm -rf build && \
    mkdir build && cd build && \
    cmake .. && \
    cmake --build . --target AuthService --config Release

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libcurl4 \
    openssl \
    zlib1g \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/build/AuthService .

EXPOSE 8081
CMD ["./AuthService"]
