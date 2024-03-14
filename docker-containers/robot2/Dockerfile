FROM osrf/ros:humble-desktop-jammy

RUN apt-get update && apt-get install -y \
   git \
   ros-humble-diagnostic-updater \
   ros-humble-laser-proc \
   ros-humble-urg-c \
   build-essential \
   python3-colcon-common-extensions \
   python3-pip \
   ros-humble-std-srvs \
   ros-humble-ament-lint-auto \
   && pip install pyserial \
   && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

RUN git clone -b ros2-devel https://github.com/ros-drivers/urg_node.git src/urg_node
RUN git clone https://github.com/dawonn/vectornav.git -b ros2 src/vectornav
RUN git clone https://github.com/fadlio/kinect_ros2 src/kinect_ros2

RUN git clone https://github.com/ros-perception/perception_pcl.git -b ros2 src/perception_pcl
RUN git clone https://github.com/ros2/urg_node_msgs.git src/urg_node_msgs

RUN rosdep update && \
   rosdep install -y --from-paths src --ignore-src --skip-keys="urg_node_msgs depth_image_proc" && \
   rm -rf /var/lib/apt/lists/*

RUN . /opt/ros/humble/setup.sh && \
   colcon build --symlink-install --packages-up-to depth_image_proc urg_node_msgs && \
   colcon build

RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
RUN echo "source /workspace/install/setup.bash" >> ~/.bashrc

ENV ROS_WS=/workspace

ENTRYPOINT ["/bin/bash"]
CMD []
