# TexTime

TexTime is a DIY word clock: a steel faceplate with the letters cut out, a matrix of
addressable RGB LEDs behind it, and an ESP8266 that tells the time in words, in French
or English. It comes in two sizes, a 40 × 40 cm clock that can be 3D-printed and
assembled at home, and a 100 × 100 cm wall piece built on laser-cut MDF.

![TexTime](docs/images/textime.jpg)

Everything needed to build one is in this repository: the firmware, the electronics,
the mechanical files, the bills of materials and the build guides.

**A word of caution.** This repository gathers, after the fact, the pieces of a project
that is several years old: firmware, design files, notes and photos brought together
into one place. Some of the information may be inaccurate or out of date, in particular
supplier references, part availability, prices and online tools. Check before ordering,
and treat the guides as a starting point rather than a guarantee.

## What is in here

| Folder | Content |
|---|---|
| [`firmware/TexTime`](firmware/TexTime) | Arduino sketch for the ESP8266 (NodeMCU). Web interface, MQTT, NTP, scheduler, animations, games. Pre-built binaries are on the [releases page](../../releases). |
| [`hardware/electronics/pcb-multi-ledstripe`](hardware/electronics/pcb-multi-ledstripe) | The control board, common to both sizes when the LEDs are a strip: schematic, gerbers, bill of materials, JLCPCB SMT files. |
| [`hardware/electronics/pcb-40x40-integrated`](hardware/electronics/pcb-40x40-integrated) | The 40 × 40 integrated board: one PCB carrying the 114 WS2813 LEDs and the control electronics, instead of a cut-up strip. Both boards can be made at [JLCPCB](https://jlcpcb.com/) for a few euros from the gerber files. |
| [`hardware/mechanical/40x40`](hardware/mechanical/40x40) | Faceplate DXF, 3D-printed frame STL (one piece or four parts), back cover DXF. |
| [`hardware/mechanical/100x100`](hardware/mechanical/100x100) | Faceplate DXF and the three laser-cut MDF layers of the LED support. |
| [`bom`](bom) | Bill of materials for each size, with supplier references (French). |
| [`docs`](docs) | Build guides (French). |

## The two sizes

|  | 40 × 40 | 100 × 100 |
|---|---|---|
| Faceplate | Steel S235, 1 mm, laser cut, held by 24 magnets | Steel S235, 1 mm, laser cut |
| Structure | 3D-printed PLA frame, one piece or four | Five glued laser-cut MDF plates (3 + 3 × 10 + 3 mm), no frame |
| LEDs | Strip, 30 LED/m, 114 LEDs, or the integrated board | Strip, two LEDs per letter, folded at the end of each row |
| Control | Control board with the strip; with the integrated board it is already on it, nothing extra to make | Control board |
| Power | 5 V 5 A | 5 V 10 A |
| Firmware "Clock type" | `40x40@1` | `100x100@3` (folded strip), `@2` or `@1` (cut and joined) |

For the strip, SK6812 RGBW 5 V is the better choice today: better quality than
WS2812/WS2813, and its dedicated white LED is used for white text. WS2813 still works.
The strip type is selected in the settings, "LED type".

## Building one

1. Read the build guide: [`docs/build-guide-40x40.md`](docs/build-guide-40x40.md) or [`docs/build-guide-100x100.md`](docs/build-guide-100x100.md).
2. Order the parts from the bill of materials in [`bom`](bom).
3. Flash the latest firmware from the [releases](../../releases), or build it yourself, see [`firmware/TexTime/README.md`](firmware/TexTime/README.md).
4. On first boot the clock opens a `TexTime-xxxx` WiFi network; connect to it, open `http://192.168.1.1/` and enter your WiFi settings.

This is a personal project, shared so that others can build their own. It comes without
any guarantee.

## License

Everything here, firmware, hardware files, documentation and images, is released under
[CC0 1.0 Universal](LICENSE): public domain, no rights reserved, no attribution
required. Use it, change it, sell it. TexTime is itself a reproduction of an existing
word-clock design, and no ownership is claimed on the idea.
