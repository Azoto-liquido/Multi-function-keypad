# Multi Function Keypad – QMK Firmware

QMK firmware for a custom keyboard based on the XIAO RP2040, featuring:
- 4 Cherry MX switches
- EC11 rotary encoder
- SSD1306 OLED display 128×64
- 2 SK6812MINI RGBW LEDs

---

## Pinout

| Component         | GPIO  | Notes                              |
|-------------------|-------|------------------------------------|
| SW1               | GP1   | Cherry MX switch                   |
| SW2               | GP2   | Cherry MX switch                   |
| SW3               | GP4   | Cherry MX switch                   |
| SW4               | GP3   | Cherry MX switch                   |
| Encoder A         | GP29  | EC11 channel A                     |
| Encoder B         | GP28  | EC11 channel B                     |
| Encoder Switch S1 | GP26  | Push button (internal pull-up)     |
| Encoder Switch S2 | GP27  | Connected to GND on PCB            |
| OLED SDA          | GP6   | I2C1, SSD1306 128×64               |
| OLED SCL          | GP7   | I2C1                               |
| LED DIN           | GP0   | SK6812MINI RGBW, chain: D2 → D1   |

---

## Features

### Rotary Encoder EC11

| Action           | Layer BASE        | Layer FN1           |
|------------------|-------------------|---------------------|
| Turn clockwise   | Volume Up         | LED Brightness +    |
| Turn counter-CW  | Volume Down       | LED Brightness -    |
| Click (GP26)     | Mute toggle       | Mute toggle         |

### Keys – Layer BASE

| Key | Function       |
|-----|----------------|
| SW1 | Play / Pause   |
| SW2 | Stop           |
| SW3 | Previous track |
| SW4 | Hold → FN1     |

### Keys – Layer FN1

| Key | Function            |
|-----|---------------------|
| SW1 | Next track          |
| SW2 | Mute toggle         |
| SW3 | OLED text input     |
| SW4 | (held for FN1)      |

### OLED Display SSD1306

The display renders 4 rows (6×8 font, 21 columns):

| Row | Content                              |
|-----|--------------------------------------|
| 0   | Fixed title "Multi Keypad"           |
| 1   | Active layer number                  |
| 2   | Volume status: `active` / `[MUTE]`   |
| 3   | Custom scrolling text                |

**Scrolling behaviour:**
- Text ≤ 21 characters → centered and static
- Text > 21 characters → scrolls right to left in a continuous loop

### Setting the OLED text

**Method 1 – From the keyboard (keycode `OLED_IN`):**
1. Hold SW4 (FN1) and press SW3 → input mode activates
2. A blinking cursor appears on row 3
3. Type the desired text from the host PC
4. **Enter** = confirm | **Esc** = cancel | **Backspace** = delete

**Method 2 – From VIA:**
1. Open VIA → **OLED** tab → **Scrolling Text** field
2. Type the text and save

**Method 3 – Compile-time default in `config.h`:**
```c
#define OLED_SCROLL_TEXT "Insert text..."
```

### SK6812MINI LEDs
- 2 RGBW underglow LEDs
- Controllable from VIA in the **Lighting** tab
- On FN1 layer: encoder knob adjusts brightness
- Available effects: breathing, cycle all, cycle left-right, rainbow beacon, jellybean raindrops, pixel rain

---

## Building

```bash
# Copy the keyboard folder into your QMK installation
cp -r multi_function_keypad ~/qmk_firmware/keyboards/

# Compile
qmk compile -kb multi_function_keypad -km via

# Flash (hold BOOT on XIAO while plugging USB, then drag the .uf2 to RPI-RP2)
qmk flash -kb multi_function_keypad -km via
```

---

## File structure

```
multi_function_keypad/
├── config.h                   # Pin definitions and parameters
├── halconf.h                  # Enable I2C in ChibiOS HAL
├── mcuconf.h                  # Enable I2C1 on RP2040
├── info.json                  # Matrix, encoder, RGB matrix, layout
├── multi_function_keypad.h    # Public API header
├── multi_function_keypad.c    # OLED rendering, LED config
└── keymaps/via/
    ├── keymap.c               # Key logic, encoder, mute, OLED input
    ├── rules.mk               # Enable VIA and ENCODER_MAP
    └── via.json               # Load into VIA for key remapping
```
