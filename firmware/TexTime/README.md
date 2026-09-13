# TexTime

TexTime project is a DIY word clock project based on an ESP8266 MCU and WS2813 LEDs. 
The clock displays time using illuminated words rather than traditional digits, with several features like : 
- Support for multiple languages (French and English).
- Web interface configuration
- MQTT support
- Automatic NTP time synchronization with timezone and daylight saving
- Multiple matrix configurations (40x40, 100x100 variants)
- Selectable LED type: WS2812/WS2813 (default), SK6812 RGB and RGBW, WS2811, APA106
- Animation effects: Blink, Fire, Matrix, Rainbow, and more
- Color randomization modes
- Automatic brightness based on ambient light
- OTA firmware update

![TexTime](../../docs/images/textime.jpg)


## 👷 Build Requirements

To build this code, you need the following Arduino libraries:
- **NeoPixelBus by Makuna**: https://github.com/Makuna/NeoPixelBus
- **Rtc by Makuna**: https://github.com/Makuna/Rtc

The hardware (electronics, mechanical files, bills of materials) and the build guides
are in the parent folders of this repository.

## 🤖 Development Credits

The modern web interface redesign and optimization were implemented by **Claude AI** (Anthropic), providing:
- Complete frontend/backend refactoring
- Modern responsive design implementation
- Performance optimization for ESP8266
- Enhanced user experience features
- Comprehensive code cleanup and documentation

---

*TexTime continues to evolve with modern web technologies while maintaining its DIY spirit and Arduino compatibility.*