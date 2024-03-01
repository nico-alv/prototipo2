FROM dustynv/ros:humble-desktop-l4t-r35.4.1
RUN sudo apt update
RUN sudo apt install screen
RUN sudo pip3 install Jetson.GPIO
RUN sudo pip3 install pyserial 
COPY ./entrypoint.sh /home/entrypoint.sh
RUN chmod +x /home/entrypoint.sh
WORKDIR /home/ros2_ws
#ENTRYPOINT ["/bin/bash", "-c", "/home/entrypoint.sh"]
