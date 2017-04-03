# Camera Streaming Daemon

[![Build Status](https://travis-ci.org/01org/camera-streaming-daemon.svg?branch=master)](https://travis-ci.org/01org/camera-streaming-daemon)

## Pre-Requisites
In order to compile you need the following packages:

 - GCC/G++ compiler 4.9 or newer
 - C and C++ standard libraries
 - GLib 2.42 or newer (https://wiki.gnome.org/Projects/GLib)
 - Avahi 0.6 or newer (https://github.com/lathiat/avahi)
 - GStreamer 1.4 or newer (https://gstreamer.freedesktop.org/)
 - GStreamer RTSP Server 1.4 or newer (https://gstreamer.freedesktop.org/modules/gst-rtsp-server.html)


## Build and Install

Build system follows the usual configure/build/install cycle. Configuration is needed to be done only once. A typical configuration is shown below:

    $ ./autogen.sh && ./configure \
        --sysconfdir=/etc --localstatedir=/var --libdir=/usr/lib64 \
        --prefix=/usr
Build:

    $ make

Install:

    $ make install
    $ # or... to another root directory:
    $ make DESTDIR=/tmp/root/dir install

## Running

Camera streaming daemon is composed of a single binary that can be run without any aditional parameter:

    $ camera-streaming-daemon

## Testing

Currently there are no Ground Control System with full support for Camera Streaming Daemon, but there are 2 straightforward ways to test the daemon, using a WIP implementation in QGroundControl or camera-sample-client sample + vlc.

### QGroundControl

There is a QGroundControl branch, still under development, but functional on Linux, to discovery Camera Streaming Daemon streams and play them.

In order to use it, close the repository located at https://github.com/otaviobp/qgroundcontrol/tree/camera-streaming-daemon-support:

    $ git clone -b camera-streaming-daemon-support https://github.com/otaviobp/qgroundcontrol.git

And build it, following the instructions in QGroundControl website: http://qgroundcontrol.org/dev/build_qgc_new

This modified version of QGroundControl adds an option called "Zeroconf Cameras" another option to "VideoSource" combobox in General Settings panel.

[[images/qgc1.png|alt=QGroundControl Video Settings]]

As soon as this option is selected, information and options about the video is loaded in "Video Streaming Widget", accessible in the Widgets menu (Widgets->Video Streaming).

[[images/qgc2.png|alt=QGroundControl Video Streaming Widget]]

In Video Streaming widget, select the desired video stream and, optionally, format and resolution. The video will start playing in Video Area automatically.

### Sample + vlc

#### Running the sample
In camera-streaming-daemon samples repository there is one sample to list all drone's video stream found.
The first step to build the samples:

    $ make samples

And then in a computer connected to the target drone using WiFi or any other Ethernet connection:

    $ ./samples/camera-sample-client

camera-sample-client will list all discovered streams in an output like:

    (main_sample_client.cpp:110) Camera Streaming Client
    (main_sample_client.cpp:48) Service resolved: '/video0' (rtsp://192.168.1.1:8554/video0)
    (main_sample_client.cpp:51) TXT: ["name=Integrated_Webcam_HD"
    "frame_size[0]=MJPG(848x480,960x540,1280x720)"]

#### Play the video using VLC

In order to play the discovered stream, copy the video URI and open it using the vlc video player (or any other video player that supports rtsp):

    $ vlc rtsp://192.168.1.1:8554/video0

VLC will start streaming your video.

## Contributing

Pull-requests are accepted on GitHub. Make sure to check coding style with the
provided script in tools/checkpatch, check for memory leaks with valgrind and
test on real hardware.

### Valgrind

In order to avoid seeing a lot of glib and gstreamer false positives memory leaks it is recommended to run valgrind using the followind command:
    $ GDEBUG=gc-friendly G_SLICE=always-malloc valgrind --suppressions=valgrind.supp --leak-check=full --track-origins=yes --show-possibly-lost=no --num-callers=20 ./camera-streaming-daemon
