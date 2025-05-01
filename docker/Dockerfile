ARG OS=ubuntu
FROM $OS:latest

ENV DEBIAN_FRONTEND=noninteractive
ENV LC_ALL=de_DE.UTF-8
ENV LANG=de_DE.UTF-8
ENV TZ=Europe/Berlin
RUN apt update && apt dist-upgrade -y && apt install -y \
   build-essential autoconf git doxygen cppcheck locales libcppunit-dev libgtest-dev joe \
   libopenexr-dev libilmbase-dev libjpeg-dev libexiv2-dev libtiff5-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libxt-dev libglu1-mesa-dev freeglut3-dev \
   qtbase5-dev qhelpgenerator-qt5 \
   qt6-base-dev qt6-documentation-tools
RUN locale-gen de_DE.UTF-8

RUN addgroup dev
RUN adduser --ingroup dev --home /home/dev --gecos "Development user" --disabled-password dev
USER dev
WORKDIR /home/dev
RUN git clone https://github.com/stmork/blz3.git
