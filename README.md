## Kickstarter Tracker with UDOO NEO

This a simple KS tracking project that uses **[UDOO NEO][udoo]**'s application core with a
little server written in Python and an Arduino sketch that runs on the M4
processor.

![box][box]

### Features

* 3 status menus
  * Total pledged status
  * Total backers
  * Time left to back the project
* Buzzes when pledged total changes
* Shows temporally how much pledge difference between updates
* Check KS in a fixed delay

### Circuit

![circuit][circuit]

This project needs:
* UDOO NEO
* LCD 16x2 Hitachi HD44780 compatible
* Piezo Buzzer
* Button
* 10kΩ Resistor
* 10kΩ Pot
* Breadboard wires

### Usage

Flash the precompiled sketch into the UDOO Neo or compile & flash from source
(change header for a different project).

Install dipendences:
`# apt install python3 python3-pycurl python3-serial python3-bs4 python3-html5lib`

Execute the server `ks_neo.py` editing the config variables at the top of the script.  
By default it monitors the [UDOO X86][udoox86] Kickstarter page.

### Install

If you want to make it start at boot, and if you're using **UDOObuntu2rc2**,
install the service file `neo_ks.conf` in `/etc/init/`, and `ks_neo.py` in
`/usr/bin`.

### Have fun!

Written under **GPL licence**

[udoo]: http://www.udoo.org
[circuit]: ks_neo_bb.png
[box]: ks_neo_box.jpg
[udoox86]: https://www.kickstarter.com/projects/udoo/udoo-x86-the-most-powerful-maker-board-ever
