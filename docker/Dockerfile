FROM ubuntu:jammy

# Update Installed
RUN apt update
RUN apt upgrade -y

# Set Locale (some dependencies will ask otherwise)
RUN apt install -y locales
RUN locale-gen en_US.UTF-8
RUN ln -s /usr/share/zoneinfo/Europe/Vienna /etc/localtime

# Install Yocto Dependencies
RUN apt install -y \
    gawk \
    wget \
    git \
    diffstat \
    unzip \
    texinfo \
    gcc \
    build-essential \
    chrpath \
    socat \
    cpio \
    python3 \
    python3-pip \
    python3-pexpect \
    xz-utils \
    debianutils \
    iputils-ping \
    python3-git \
    libegl1-mesa \
    libsdl1.2-dev \
    pylint \
    xterm \
    python3-subunit \
    mesa-common-dev \
    zstd \
    liblz4-tool \
    python3-jinja2 \
    file \
    openssh-server

# Create a User
RUN groupadd -g 1000 dev 
RUN useradd -u 1000 -g dev -d /home/dev dev 
RUN mkdir /home/dev && chown -R dev:dev /home/dev

# Set up SSH 
RUN echo 'dev:yocto' | chpasswd
RUN echo 'root:yocto' | chpasswd

RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
RUN mkdir /var/run/sshd
RUN ssh-keygen -A

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]