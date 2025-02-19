# Proof of Concept: Building a Unikraft Image on Raspberry Pi 5

## Repository Overview
This repository contains the necessary files to build a Unikraft unikernel on a Raspberry Pi 5 using Docker.

### Repository File Structure
```
repo/
│-- Dockerfile                 # Docker configuration for building the unikernel
│-- Kraftfile                  # Unikraft build configuration
│-- build.sh                   # Shell script to automate the build process
│-- Makefile.uk                # Makefile for Unikraft build system
│-- http.cpp                   # Sample HTTP server code
│-- .dockerignore              # Files to exclude from Docker build
│-- Readme.md                  # Documentation
│-- working_build_30.01.2025.log   # Log from a successful build without memory issues(30.01.2025)
│-- log_with_very_high_memory_usage.log # Log from a build after memory increase (19.02.2025)
```

## Setup & Expected Behavior
The expected workflow is as follows:
1. Clone this repository onto a Raspberry Pi 5 with Docker installed.
2. Run the command:
   ```sh
   docker build . -t test
   ```
3. The build should complete successfully, as it did on **30.01.2025** (refer to `working_build_30.01.2025.log`).
4. The successful build was executed on a Raspberry Pi 5 with 8GB Ram and other processes running (using about 2GB ram).

## Issue: Build Fails Due to Memory Constraints (As of 19.02.2025)
When attempting to rebuild the same setup on **19.02.2025**, the build fails with an **out-of-memory (OOM) error** during the linking stage.

### Error Logs
During the build, the following linker error occurs:
```
level=info msg="  LD      libmusl.ld.o"
level=info msg="  OBJCOPY libmusl.o"
level=info msg="  LD      mytcpsever_qemu-arm64.dbg"
level=warning msg="/usr/bin/ld: warning: -z relro ignored"
level=warning msg="collect2: fatal error: ld terminated with signal 9 [Killed]"
level=warning msg="compilation terminated."
```

in journalctl noted as
```
kernel: Out of memory: Killed process 115521 (ld) total-vm:9126928kB, anon-rss:6479184kB, file-rss:0kB, shmem-rss:0kB, UID:0 pgtables:3904kB oom_score_adj:0
```

### Observations
- Monitoring `htop` during the build shows that the linker is consuming **excessive memory**, causing the process to be killed.
- Increasing the swap space from **512MB to 2GB** allows the build to *barely* complete, but the system becomes unresponsive.
- If any additional background processes (e.g., a utility service using 1-2GB of RAM) are running, the build is inevitably **killed due to OOM conditions**.

### Memory Usage Details (from `journalctl`)
```
Total virtual memory (total-vm): 9126928 kB (~8.7 GB)
Anonymous resident memory (anon-rss): 6479184 kB (~6.2 GB)
Page tables memory (pgtables): 3904 kB

 23006 root        20   0 8922M 7411M   512 D   6.5 92.0  0:06.65 /usr/bin/ld -plugin /usr/libexec/gcc/aarch64-linux-gnu/13/liblto_plugin.so \
   -plugin-opt=/usr/libexec/gcc/aarch64-linux-gnu/13/lto-wrapper -plugin-opt=-fresolution=/tmp/ccPpoOus.res --build-id --eh-frame-hdr \
   --hash-style=gnu --as-needed -dynamic-linker /lib/ld-linux-aarch64.so.1 -X -EL -maarch64linux --fix-cortex-a53-843419 -z relro \
   -o /builds/jzi-master/unikernel-unikraft/http/.unikraft/build/mytcpsever_qemu-arm64.dbg -L/usr/lib/gcc/aarch64-linux-gnu/13 -L/usr/li
```

## Request for Assistance
This issue did not occur on **30.01.2025**, but the same setup now fails on **19.02.2025**. It appears to be a regression or a change affecting memory usage.

- **Has there been any known increase in linker memory usage?**
- **Are there recommended workarounds to reduce memory consumption during linking?**

Any guidance or suggestions for debugging this issue further would be greatly appreciated!

