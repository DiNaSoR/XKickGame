FROM ubuntu:22.04

RUN dpkg --add-architecture i386 \
    && printf '%s\n' \
      'deb [arch=i386 trusted=yes] http://old-releases.ubuntu.com/ubuntu/ lucid main universe' \
      'deb [arch=i386 trusted=yes] http://old-releases.ubuntu.com/ubuntu/ lucid-updates main universe' \
      'deb [arch=i386 trusted=yes] http://old-releases.ubuntu.com/ubuntu/ lucid-security main universe' \
      > /etc/apt/sources.list.d/lucid.list \
    && apt-get -o Acquire::Check-Valid-Until=false update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      libc6:i386 \
      libstdc++6:i386 \
      libgcc-s1:i386 \
      libnsl2:i386 \
      libmysqlclient16:i386 \
      procps \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /home/xkick
