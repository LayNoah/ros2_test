#!/bin/bash

docker build \
	--network host \
	-t ros2_humble_cuda12.8_noah \
	-f Dockerfile \
	.
