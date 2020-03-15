# ImageCapture

The main idea of this project is to create a video device remote control system.

Client sends command to Server (in my case BeagleBone Black) to:

      Capture image from connected to Server video device;
      Process image (grayscale);
      Get processing results (for now just hardcoded integers);

Environment:

    OS: Linux
    IDE: Eclipse

ImageCapture - static library that provides C++ wrapper over video device (VideoDevice class).

ClientServerCommon - noexcept header only library with some common classes and functions and definitions used in Client and Server projects

    Socket - C++ wrapper over socket
  
Utils - noexcept header only library with RAII wrappers and helper functions for handling async calls to video device

Client tested on Ubuntu 16.04 LTS

Server tested on BeagleBone Black (OS Debian 9.5 2018-10-07 4GB eMMC IoT Flasher)

Feel free to contact me with questions and suggestions:

mark.rakhmatov@gmail.com
