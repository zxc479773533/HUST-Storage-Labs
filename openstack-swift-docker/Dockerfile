FROM phusion/baseimage:0.9.22
MAINTAINER Zhan.Shi <g.shizhan.g@gmail.com>

RUN apt-get update && apt-get install -y \
        memcached rsync pwgen supervisor \
        python-xattr python-memcache python-netifaces \
        swift python-swiftclient swift-plugin-s3 \
        swift-account swift-proxy swift-object swift-container \
        python-keystoneclient

RUN mkdir -p /var/log/supervisor
ADD files/supervisord.conf /etc/supervisor/conf.d/supervisord.conf

ADD files/dispersion.conf /etc/swift/dispersion.conf
ADD files/rsyncd.conf /etc/rsyncd.conf
ADD files/swift.conf /etc/swift/swift.conf
ADD files/proxy-server.conf /etc/swift/proxy-server.conf
ADD files/account-server.conf /etc/swift/account-server.conf
ADD files/object-server.conf /etc/swift/object-server.conf
ADD files/container-server.conf /etc/swift/container-server.conf
ADD files/proxy-server.conf /etc/swift/proxy-server.conf
ADD files/startmain.sh /usr/local/bin/startmain.sh
RUN chmod 755 /usr/local/bin/*.sh

EXPOSE 8080

CMD /usr/local/bin/startmain.sh
