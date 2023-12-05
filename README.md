# Motion Mouse

Motion Mouse uses a microcontroller and its built-in accelerometer, along with an external IR array sensor and a haptic driver, to allow you to control your computer's cursor with hand/wrist motions and gestures.

Motion Mouse was built for Northwestern University's **COMP_ENG 346: Microprocessor System Design** course in Fall 2023.

## Software

- [**Onboard Software**](software/apps/motion_mouse/) that runs on the microcontroller
- [**Interaction Driver**](mouse_driver/) that runs on the computer

## Hardware

### Models

- [**Custom-built 3D-printed enclosure**](https://cad.onshape.com/documents/1f374a1d7cd6b85a25884c38/w/b78bf1843882e2c558f02bcb/e/4411117a622a65413be4be04?renderMode=0&uiState=656ed59a93369b20157422b8)

### Products

- [**micro:bit v2**](https://microbit.org/new-microbit/) with serial connection to computer
- [**SparkFun Qwiic micro:bit Breakout**](https://www.sparkfun.com/products/16445) (we used [one with headers](https://www.sparkfun.com/products/16446), but the headers were left unused)
- [**SparkFun Grid-EYE Infrared Array Breakout - AMG8833 (Qwiic)**](https://www.sparkfun.com/products/14607)
- [**SparkFun Qwiic Haptic Driver - DA7280**](https://www.sparkfun.com/products/17590)
- [**Qwiic Cables**](https://www.sparkfun.com/categories/tags/qwiic-cables) (various lengths, minimum 2)
- [**Sport Bands compatible with Apple Watch**](https://www.amazon.com/gp/product/B09KNCDSWN/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1) (or similar)

## Usage

### Setup

1. Connect the micro:bit breakout to the micro:bit.
2. Connect the infrared array breakout to the micro:bit breakout via a Qwiic cable.
3. Connect the haptic driver to the micro:bit breakout via a Qwiic cable.
4. Connect the micro:bit to the computer via USB.
5. Load the [onboard software](software/apps/motion_mouse/) onto the micro:bit.

### Usage

1. Connect the micro:bit to the computer via USB.
2. Run the [interaction driver](mouse_driver/) on the computer.


## Authors

Motion Mouse was built by some students in Northwestern University's class of 2024!

- [**Dilan Nair**](https://github.com/dilanx)
- [**Sahana Vandayar**](https://github.com/sahanamvandayar)
- [**Naomi Wu**](https://github.com/nawu1552)

---

*This repository is built on top of [nu-microbit-base](https://github.com/nu-ce346/nu-microbit-base), which is built on top of [nrfx52-base](https://github.com/lab11/nrf52x-base).*

*See [README-nu-microbit-base.md](README-nu-microbit-base.md).*
