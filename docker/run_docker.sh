#!/bin/bash

# Setup X11 authentication only if not already done
if [ ! -f /tmp/.docker.xauth ]; then
    touch /tmp/.docker.xauth
    xauth nlist $DISPLAY | sed -e 's/^..../ffff/' | \
    xauth -f /tmp/.docker.xauth nmerge -
    chmod 644 /tmp/.docker.xauth

    # Allow Docker containers to access X11
    xhost +local:docker
fi

CONTAINER_NAME="ros2_humble_test_noah"

# Check if container exists and is running
if docker ps -a --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
    if docker ps --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
        # Container is running, open a new terminal in it
        echo "Container is already running. Opening new terminal..."
        docker exec -it $CONTAINER_NAME /bin/bash
    else
        # Container exists but is stopped, remove it
        echo "Removing stopped container..."
        docker rm $CONTAINER_NAME
        # Start new container (fall through to docker run below)
    fi
else
    # Container doesn't exist, start new one (fall through to docker run below)
    :
fi

# Run Docker container with X11 support (only if container didn't exist or was stopped)
if ! docker ps --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
    docker run -it --rm \
          --privileged \
          --network host \
          --runtime nvidia \
          --gpus all \
          -e DISPLAY=$DISPLAY \
          -e NVIDIA_VISIBLE_DEVICES=all \
          -e NVIDIA_DRIVER_CAPABILITIES=all \
          -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
          -e XAUTHORITY=/tmp/.docker.xauth \
          -v /tmp/.docker.xauth:/tmp/.docker.xauth:rw \
          -v /home/noah/humble:/humble \
          --name $CONTAINER_NAME \
          ros2_humble_cuda12.8_noah \
          /bin/bash
fi
