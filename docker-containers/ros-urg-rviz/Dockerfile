FROM kristoferb/spweb_ros2

RUN apt-get update && apt update && apt-get install -y \
    git \
    ros-humble-diagnostic-updater \
    ros-humble-laser-proc \
    ros-humble-urg-c \
    build-essential \
    python3-colcon-common-extensions \
    python3-pip \
    ros-humble-std-srvs \
    ros-humble-urg-node-msgs \
    ros-humble-ament-lint-auto \
    && pip install pyserial \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
RUN git clone -b ros2-devel https://github.com/ros-drivers/urg_node.git src/urg_node

RUN apt-get update && rosdep update && rosdep install -y \
    --from-paths src/urg_node \
    --ignore-src \
    && apt-get install -y ros-humble-joy \
    && rm -rf /var/lib/apt/lists/*

RUN . /opt/ros/humble/setup.sh && \
    colcon build --packages-select urg_node

RUN echo '#!/bin/bash\n\
source /opt/ros/humble/setup.sh\n\
source $ROS_WS/install/setup.sh\n\
exec "$@"' > /ros_entrypoint.sh && \
chmod +x /ros_entrypoint.sh

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]

ENV ROS_WS=/workspace

