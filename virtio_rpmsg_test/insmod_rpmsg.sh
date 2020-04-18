#!/bin/bash
sudo dmesg -C; sudo insmod ./rpmsg_core.ko; sudo insmod ./rpmsg_char.ko; sudo insmod ./virtio_rpmsg_bus.ko
