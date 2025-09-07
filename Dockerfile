FROM alpine:3.19.0 AS build

RUN apk add --no-cache \
    gcc \
    git \
    tar \
    gzip \
    musl-dev \
    cmake \
    make \
    json-c-dev \
    libmaxminddb-dev \
    libmaxminddb-static \
    gnutls \
    gnutls-dev

# alpine's libmicrohttpd is compiled with https support enabled, but they don't offer
# static gnutls builds :(
WORKDIR /usr/src
RUN wget https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-1.0.2.tar.gz
RUN [ $(sha256sum libmicrohttpd-1.0.2.tar.gz | cut -d' ' -f1) == "df324fcd0834175dab07483133902d9774a605bfa298025f69883288fd20a8c7" ]
RUN tar xvf libmicrohttpd-1.0.2.tar.gz
RUN rm libmicrohttpd-1.0.2.tar.gz

WORKDIR /usr/src/libmicrohttpd-1.0.2
RUN ./configure --disable-https --prefix=/usr
RUN make -j`nproc`
RUN make install

COPY . /usr/src/tracker
RUN mkdir /usr/src/tracker/build

# enet only uses C, but since it doesn't specify "LANGUAGES C" then cmake checks for a
# C++ compiler.. so let's patch that out
WORKDIR /usr/src/tracker
RUN sed -i \
    's/project(enet)/project(enet LANGUAGES C)/g' \
    Extern/enet6/CMakeLists.txt

# compile!
WORKDIR /usr/src/tracker/build
RUN LDFLAGS="-static" cmake .. -DHARDENED=ON
RUN make -j`nproc`
RUN mkdir /app
RUN cp Tracker /app/tracker

# statically built, so we only need the executable now
FROM scratch AS runtime

COPY --from=build \
    /app \
    /app

EXPOSE 32886/udp
EXPOSE 32885/tcp

WORKDIR /app/data
CMD [ "/app/tracker" ]
