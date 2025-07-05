# TexTime

TexTime project is a DIY word clock project based on an ESP8266 MCU and WS2813 LEDs. 
The clock displays time using illuminated words rather than traditional digits, with several features like : 
- Support for multiple languages (French and English).
- Web interface configuration
- MQTT support
- Automatic NTP time synchronization with timezone and daylight saving
- Multiple matrix configurations (40x40, 100x100 variants)
- Animation effects: Blink, Fire, Matrix, Rainbow, and more
- Color randomization modes
- Automatic brightness based on ambient light
- OTA firmware update

![TexTime](https://i.ibb.co/tMPVx2w/76635672-2540162132869398-177845958548127744-o.jpg)


## 👷 Build Requirements

To build this code, you need the following Arduino libraries:
- **NeoPixelBus by Makuna**: https://github.com/Makuna/NeoPixelBus
- **Rtc by Makuna**: https://github.com/Makuna/Rtc


## 🔗 Documentation & Resources

- **Hardware Documentation**: https://easyeda.com/psykokwak/TexTime-V3
- **Project Repository**: https://github.com/psykokwak-com/TexTime

## 🤖 Development Credits

The modern web interface redesign and optimization were implemented by **Claude AI** (Anthropic), providing:
- Complete frontend/backend refactoring
- Modern responsive design implementation
- Performance optimization for ESP8266
- Enhanced user experience features
- Comprehensive code cleanup and documentation

---

*TexTime continues to evolve with modern web technologies while maintaining its DIY spirit and Arduino compatibility.*