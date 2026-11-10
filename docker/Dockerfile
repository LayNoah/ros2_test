# ===== Base Image (CUDA 12.8 + Ubuntu 22.04) =====
FROM nvidia/cuda:12.8.0-devel-ubuntu22.04

# Change Ubuntu mirrors to mirror.kakao.com
RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://mirror.kakao.com/ubuntu|g' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu|http://mirror.kakao.com/ubuntu|g' /etc/apt/sources.list && \
    sed -i 's|http://ports.ubuntu.com/ubuntu-ports|http://mirror.kakao.com/ubuntu-ports|g' /etc/apt/sources.list


# apt 설치 중에 "사용자 입력 프롬프트를 막는 설정" -> 알아서 설치해라!
ENV DEBIAN_FRONTEND=noninteractive

# docker의 기본 쉘을 /bin/sh -> /bin/bash로 바꾸는 설정.
SHELL ["/bin/bash", "-c"]


# locales: 언어 설정용
# curl, wget: 스크립트/파일 다운로드
# cmake, build-essential: C++ 빌드를 위함. (ROS2 패키지 빌드용)
# python3, python3-pip, python3-dev: ROS2 Python 빌드 및 실행
# python3-colcon-common-extensions: ROS2 빌드 툴(colcon)
# python3-argcomplete: 자동완성 기능
# gnupg2, lsb-release: ROS apt key 추가에 필요
# rm -rf /var/lib/apt/lists/* -> apt 캐시 삭제. 이미지 용량 감소
# ===== Basic dependencies =====
RUN apt-get update && apt-get install -y \
    locales \
    curl \
    wget \
    git \
    vim \
    cmake \
    build-essential \
    python3 \
    python3-pip \
    python3-venv \
    python3-argcomplete \
    python3-dev \
    python3-distutils \
    gnupg2 \
    lsb-release \
    apt-utils \
    sudo \
    && rm -rf /var/lib/apt/lists/*

# 컨테이너에서 문자열 인코딩 문제 방지
# ===== Locale =====
RUN locale-gen en_US en_US.UTF-8
ENV LANG=en_US.UTF-8  
ENV LANGUAGE=en_US:en  
ENV LC_ALL=en_US.UTF-8  

# universe repository 활성화
# ===== Install ROS 2 Humble (Base + Rviz2) =====
RUN apt-get update && apt-get install -y \
    software-properties-common && \
    add-apt-repository universe && \
    apt-get update

# ROS2 패키지 인증을 위한 GPG key 추가
# ROS key
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc \
    | apt-key add -

# ROS2 패키지 소스 목록 추가
RUN echo "deb http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" \
    > /etc/apt/sources.list.d/ros2.list

# ROS2 설치
RUN apt-get update && apt-get install -y \
    ros-humble-desktop \
    ros-humble-ros-base \
    python3-rosdep \
    python3-colcon-common-extensions \
    && rm -rf /var/lib/apt/lists/*

# rosdep init
RUN rosdep init || true
RUN rosdep update

# ===== Environment Setup =====
ENV ROS_DISTRO=humble
RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
RUN echo "export RMW_IMPLEMENTATION=rmw_fastrtps_cpp" >> ~/.bashrc

# ===== Create user with UID 1000 to match host user =====
RUN groupadd -g 1000 noah && \
    useradd -u 1000 -g 1000 -m -s /bin/bash noah && \
    usermod -aG sudo noah && \
    echo "noah ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/noah

# ===== Setup user environment =====
USER noah
RUN echo "source /opt/ros/humble/setup.bash" >> /home/noah/.bashrc
RUN echo "export RMW_IMPLEMENTATION=rmw_fastrtps_cpp" >> /home/noah/.bashrc

RUN mkdir -p /home/noah/humble/workspace/src
WORKDIR /home/noah/humble/workspace

# ===== Done =====
CMD ["/bin/bash"]
