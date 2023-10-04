# TEEOD Firmware

The TEEOD firmware is responsible for running on the Enclave's Arm Cortex-M1 and providing the GP TEE Internal Core API and runtime engine.

## Project Structure
```
.
├── board
│   ├── linker
│   ├── system
│   └── uart
├── build
├── lib
│   ├── aes
│   └── sha
├── ta
└── tee
```
## Getting Started

To get started with the project:

Installation: Ensure you have the necessary tools and dependencies installed, including GCC, Make, and a suitable cross-compiler toolchain for Arm Cortex-M1.

Configuration: Set project-specific variables like ``target-app``, ``build-dir-app``, ``repo-dir-app``, ``srcs-app``, ``inc-app``, and ``DEFINES`` in the Makefile to match your project.

Memory Considerations: If more memory is required, consider modifying the linker script (board/linker/m1.ld) to accommodate your needs.

## Usage

Build: Execute make in the project directory to compile the source code and generate binary and listing files.

Execute: If applicable, use QEMU or your preferred execution environment to run the firmware.

Customization: Modify and extend the firmware to suit your specific use case, following the project's structure and guidelines.


## Troubleshooting

Include a section with common issues and their solutions or instructions for seeking help if users encounter problems.

