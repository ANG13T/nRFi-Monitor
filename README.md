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

## Image
<img src="https://github.com/angelina-tsuboi/nRFi-Monitor/blob/main/assets/preview.png" alt="n-RFi Monitor preview" width="700"/>
Testing the n-RFi Monitor at a mall

## Set up
- Arduino IDE with the following URL added under Preferences -> Additional board manager URL's: http://arduino.esp8266.com/stable/package_esp8266com_index.json

- ESP8266 boards added in Arduino IDE by going to Tools -> Boards -> Boards Manager, and installing ESP8266 Boards by ESP8266 Community

- Install Zip file from https://github.com/wonho-maker/Adafruit_SH1106 and unzip the file inside Arduino -> libraries

- Install nRF24L01 and ESP8266WiFi libraries

- Optional: Depending on which type of OLED screen you use, you might have to install this library [esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)

## Special Thanks and Resources
The n-RFi Monitor was inspired by previous projects created by Cifer Tech. Check out his projects below!
- [cifertech/nRFBox](https://github.com/cifertech/nRFBox)
- [cifertech/2.4-GHz-band-Scanner](https://github.com/cifertech/2.4-GHz-band-Scanner)

