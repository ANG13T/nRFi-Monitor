<img src="https://github.com/angelina-tsuboi/nRFi-Monitor/blob/main/assets/logo_large.png" alt="WiCon logo" width="400"/>

### A 2.4GHz band and WiFi analyzer toolkit made with the D1 Mini and NRF24L01

## Features
- 2.4 GHz Traffic Analyzer and Channel Filtering
- WiFi Scanner (Auto and Manual modes)
- WiFi Vicinity Detector with RSSI Meter
- Lipo Battery Configuration (WIP)
- Live updating RSSI Meter (WIP)

## Components
| Component | Purpose |
| --- | --- |
| Wemos D1 Mini | Monitor WiFi data & control components
| 128x64 OLED (SSH1106) | Give a visual display of Traffic and WiFi data |
| 2x Push Buttons | Gather user input |
| NRF24L01 | Get 2.4GHz Traffic Data |
| 3D Printed Case  | Protect components |
| 4x 8/32 0.625inch screws  | Hold case together |
| Lipo Battery 3.7V (optional) | Power |
| SDPT Switch (optional) | Control Battery |
| D1 Mini Shield (optional) | Battery Config |

## Breadboard
<img src="https://github.com/angelina-tsuboi/nRFi-Monitor/blob/main/assets/breadboard.png" alt="n-RFi breadboard" width="500"/>

## Schematic
<img src="https://github.com/angelina-tsuboi/nRFi-Monitor/blob/main/assets/schematic.png" alt="n-RFi schematic" width="500"/>

## Hardware Setup
**128x64 OLED (SSH1106)** 
| OLED Pin | D1 Mini |
| --- | --- |
| SCK | D1 |
| SDA | D2 |

**Left Push Button** 
| D1 Mini |
| --- |
| D4 |

**Right Push Button** 
| D1 Mini |
| --- |
| TX |

**NRF24L01** 
| NRF24 Pin | D1 Mini |
| --- | --- |
| CE | D3 |
| CSN | D8 |
| SCK | D5 |
| MO | D7 |
| MI | D6 |
