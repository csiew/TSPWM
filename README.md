# TSPWM
**TSPWM** is an X11 window manager based on [TinyWM](https://github.com/mackstann/tinywm) by [Nick Welch](https://github.com/mackstann). TinyWM was originally written in C, and has been ported to C++ for this project. The intent of this window manager project is largely to expand my understanding of the X11 libraries _and_ to finally learn some C++.

Development and testing is being done in a Debian 10.4.0 virtual machine. TSPWM is my second attempt to extending TinyWM (my first attempt was [BiscuitWM](https://github.com/csiew/BiscuitWM)).

The '_TSP_' in 'TSPWM' is an abbreviation of '[_tau sar piah_](https://www.google.com/search?q=tau+sar+piah&oq=tau+sa&aqs=chrome.1.69i57j0l6.2832j1j7&sourceid=chrome&ie=UTF-8)', my favourite biscuit native to my home state of Penang, in Malaysia.

## Install guide
Before running TSPWM, you must have the `libx11-dev` and `libxcursor-dev` libraries installed. To do so, install these libraries' packages via your Linux distribution's package manager.

To install TSPWM on your system, run the `install.sh` as `sudo` (as we need to `chmod` the scripts to run the binary files):
```bash
sudo sh install.sh
```
To run the uninstall script, run the `uninstall.sh` script as `sudo` as well:
```bash
sudo sh uninstall.sh
```

## User guide
At the moment, TSPWM follows the hybrid keyboard and mouse driven interaction from TinyWM. Future iterations will seek to add titlebars to allow for mouse-first interactivity.

Note that whichever window your cursor is hovering over will be the active window. Future iterations will require the window to be raised by clicking on the window.

### Keyboard shortcuts
- `Alt + Left Click`: Move window
- `Alt + Left Click` and drag: Raise window
- `Alt + Right Click` and drag: Resize window
- `Alt + F1`: Raise currently focused window

## Emulation guide
Instead of constantly logging off, switching the Xsession, then logging in again to test, it will be easier to just run an embedded Xsession within your current session. To do this, install the Xephyr package (`xserver-xephyr`).

### Manual
Open a terminal and run:
```bash
Xephyr -br -ac -noreset -screen 1024x780 :1 &
```
You should then see a Xephyr window popup (nothing will be visible since there is no window manager assigned). Then enter:
```bash
DISPLAY=:1
```
...to send commands to this new Xephyr window.
Then start the TSPWM session:
```bash
tspwm-session
```

Once you're done with testing, you may want to just reuse the same terminal for local commands. Thus, enter this command:
```bash
DISPLAY=:0
```
...to resume sending commands to your current Xsession.

### Partially automated*
Open a terminal in the same directory as the TSPWM files and run:
```bash
sudo sh run_dev.sh
```
Then start the TSPWM session:
```bash
tspwm-session
```
The `run_dev.sh` script will be improved in the future.