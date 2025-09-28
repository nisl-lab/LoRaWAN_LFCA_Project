# LoRaWAN MAC Layer Algorithm Research

This repository contains the source code for evaluating MAC layer algorithms in LoRaWAN networks, based on custom modifications to existing LoRaWAN implementations.

## Overview

This research investigates MAC layer collision avoidance algorithms in LoRaWAN networks following the LoRaWAN v1.1.0 specification with OTAA join procedures.

### System Architecture

- **End Devices**: NUCLEO-L476RG boards with Semtech SX1276 transceivers
- **Gateway**: Raspberry Pi 3B+ with SX1301-based concentrator module  
- **Network Server**: ChirpStack network server with PostgreSQL database

## Source Code Attribution

The implementation is based on the following open-source projects:

- **LoRaMac-node**: Modified from Semtech's reference implementation [1]
- **LoRa Gateway**: Based on Semtech's gateway library [2]
- **Packet Forwarder**: Uses Semtech's packet forwarder [3]
- **Network Server**: ChirpStack implementation [4]

## Configuration

- **Frequency Plan**: KR920 (Korean frequency plan)
- **LoRaWAN Version**: v1.1.0
- **Activation Method**: OTAA (Over-The-Air Activation)

## Project Structure

```
├── firmware/
│   └── end-device/           # Modified LoRaMac-node with custom MAC algorithms
├── gateway/                  # Gateway configuration files
├── network-server/           # ChirpStack deployment configurations  
├── analysis/                 # Data analysis and visualization scripts
└── docs/                     # Documentation and setup guides
```

## Setup Instructions

### End Device
1. Modify the LoRaMac-node firmware with custom MAC layer algorithms
2. Configure for KR920 frequency plan
3. Flash to NUCLEO-L476RG boards

### Gateway & Network Server
For gateway and network server setup, refer to the official documentation:
- **Gateway Setup**: Follow Semtech LoRa Gateway documentation [2]
- **Packet Forwarder**: Configure using Semtech Packet Forwarder guide [3]
- **ChirpStack**: Install and configure according to ChirpStack documentation [4]

## Usage

1. Deploy the modified firmware to end devices
2. Configure gateway and network server according to official documentation
3. Run experiments with different MAC layer algorithms
4. Analyze results using provided analysis scripts

## Custom MAC Algorithms

The repository includes implementations of:
- CSMA with Channel Activity Detection (CSMA-CAD)
- Q-Learning based MAC protocols
- [Additional custom algorithms as implemented]

## References

[1] M. Luis, "LoRaMac-node," Semtech Corporation, 2025. [Online]. Available: https://github.com/Lora-net/LoRaMac-node

[2] M. Coracin, "LoRa Gateway," Semtech Corporation, 2025. [Online]. Available: https://github.com/Lora-net/lora_gateway

[3] M. Coracin, "Packet Forwarder," Semtech Corporation, 2025. [Online]. Available: https://github.com/Lora-net/packet_forwarder

[4] ChirpStack, "ChirpStack Documentation," 2025. [Online]. Available: https://www.chirpstack.io/docs/

## License

This project follows the licenses of the respective source projects. Please refer to individual component licenses for details.