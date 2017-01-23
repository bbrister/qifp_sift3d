FROM ubuntu:16.04
MAINTAINER Blaine Rister <blaine@stanford.edu>
RUN apt-get update
RUN apt-get install -y git build-essential cmake zlib1g-dev liblapack-dev libdcmtk-dev libnifti-dev
WORKDIR /home/
RUN git clone -b dso --single-branch https://github.com/bbrister/SIFT3D.git
WORKDIR /home/SIFT3D/build
RUN cmake ..
RUN make && make install
WORKDIR /home/qifp-SIFT3D
ADD CMakeLists.txt miniball.cpp Miniball.hpp miniball.h sift3d_main.c siftfeature.sh ./
WORKDIR /home/qifp-SIFT3D/build
RUN cmake ..
RUN make
WORKDIR /home/qifp-SIFT3D
