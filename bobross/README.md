# bobross

Firmware for an **ESP32-S3** that gently bobs the head of a Bob Ross bobblehead
with a cheap hobby servo. The servo follows a sine wave — **±5° around center,
one cycle per second** — for a smooth, happy little nod.

## Wiring

| Servo wire        | Connect to                                  |
|-------------------|---------------------------------------------|
| Signal (yellow)   | **GPIO 13**                                 |
| Power (red)       | **5V** (VBUS/5V pin, or a separate 5V supply)|
| Ground (brown)    | **GND** (must be common with the ESP32)     |

A micro servo can usually run off the board's 5V while it's on USB. If the head
jitters or the board resets, power the servo from a separate 5V supply and tie
the grounds together.

## Build & flash (Arduino IDE)

1. Install the **ESP32 boards** package: *File → Preferences →
   Additional Board Manager URLs* →
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`,
   then *Tools → Board → Boards Manager…* → install **esp32**.
2. Install the servo library: *Tools → Manage Libraries…* → search
   **ESP32Servo** → Install.
3. Select your board under *Tools → Board → ESP32 Arduino → ESP32S3 Dev Module*
   and the correct serial port.
4. Open `bobross.ino` and click **Upload**.

### Or with `arduino-cli`

```sh
arduino-cli core install esp32:esp32
arduino-cli lib install ESP32Servo
arduino-cli compile --fqbn esp32:esp32:esp32s3 bobross
arduino-cli upload  --fqbn esp32:esp32:esp32s3 -p /dev/ttyACM0 bobross
```

## Tuning

All knobs are constants at the top of `bobross.ino`:

- `AMPLITUDE_DEG` — how far the head swings each way (default `5.0`, i.e. ±5°).
  Use `2.5` if you meant 5° peak-to-peak.
- `PERIOD_MS` — length of one full bob (default `1000` = 1 Hz).
- `CENTER_DEG` — resting position; adjust so the head sits level.
- `PULSE_MIN_US` / `PULSE_MAX_US` — servo travel limits; widen or narrow if the
  servo under- or over-travels.
