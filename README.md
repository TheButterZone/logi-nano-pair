# macOS Logitech Nano Receiver Pairing Tool

A lightweight, standalone C utility to force-pair Logitech wireless devices (like the M525 mouse) to non-unifying Logitech Nano USB Receivers (`Device ID: 0xc52f`) on macOS. 

This tool uses Apple's native **IOKit framework** to target the configuration interface (`Usage Page 0xFF00`), bypassing macOS USB write protections and avoiding the need for massive graphical libraries or Linux-specific utilities.

## Prerequisites
* **macOS Version:** Tested on macOS 10.15 Catalina (compatible with newer macOS releases).
* **Hardware:** Fits both **Intel** and **Apple Silicon (M1/M2/M3/M4)** architectures natively.
* **Ports:** Requires a USB-C adapter or hub if your Mac only has USB-C ports.
* **Compilers:** Xcode Command Line Tools installed (`xcode-select --install`).

## How to Build and Run

1. Clone or download this repository to your Mac.
2. Open your terminal, navigate to the folder, and compile the code natively:
   ```bash
   gcc -O2 -Wall -framework IOKit -framework CoreFoundation main.c -o logi_pair
   ```
3. Turn your Logitech mouse or keyboard **OFF**.
4. Run the compiled binary with administrative privileges to access the raw USB port:
   ```bash
   sudo ./logi_pair
   ```
5. The moment you see `>>> ACTIVE PAIRING MODE ENABLED <<<` on your screen, **flip your device's power switch to ON**.

The receiver hardware will catch the handshake signal and permanently save the device pairing directly onto the USB stick's chip.
