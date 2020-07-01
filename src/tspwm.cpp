#include <iostream>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
using namespace std;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define XC_left_ptr 68
#define DEBUG true

class WindowManager {
    public:
        Display *dpy;
        Window dpy_root;
        XWindowAttributes attr;
        XButtonEvent start;
        XEvent ev;
        Window last_raised_window;
        map<pid_t, Window> managed_windows = {};

        // Event observers

        void set_user_events() {
            if (DEBUG == true) {
                cout << "User events are being set..." << endl;
            }
            XGrabKey(
                dpy,
                XKeysymToKeycode(dpy, XStringToKeysym("F1")),
                Mod1Mask | Mod2Mask,
                DefaultRootWindow(dpy),
                True,
                GrabModeAsync,
                GrabModeAsync
            );
            XGrabButton(
                dpy,
                1,
                Mod1Mask | Mod2Mask,
                DefaultRootWindow(dpy),
                True,
                ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
                GrabModeAsync,
                GrabModeAsync,
                None,
                None
            );
            XGrabButton(
                dpy,
                3,
                Mod1Mask | Mod2Mask,
                DefaultRootWindow(dpy),
                True,
                ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                GrabModeAsync,
                GrabModeAsync,
                None,
                None
            );
        }

        // Decoration

        void set_cursor(Window w) {
            if (DEBUG == true) {
                cout << "Cursor decoration is being modified..." << endl;
            }
            Cursor c = XCreateFontCursor(dpy, XC_left_ptr);
            XDefineCursor(dpy, w, c);
            XFlush(dpy);
        }

        void decorate_window(Window w) {
            if (DEBUG == true) {
                cout << "Window decoration is being modified..." << endl;
            }
            unsigned long pixel = CWBorderPixel;
            XSetWindowBorder(dpy, w, pixel);
            XSetWindowBorderWidth(dpy, w, 1);
            XFlush(dpy);
        }

        // Window tracking

        void manage_window(Window w) {
            this->set_cursor(w);
            this->decorate_window(w);
            pid_t w_pid = this->get_window_pid(w);
            managed_windows[w_pid] = w;
            cout << "New window: " << w_pid << endl;
            char *name = NULL;
            if (XFetchName(dpy, w, &name) && name != NULL) {
                cout << "New window name: " << name << endl;
            }
        }

        void unmanage_window(Window w) {
            pid_t w_pid = this->get_window_pid(w);
            if (w_pid != NULL) {
                managed_windows.erase(w_pid);
                cout << "Removed window: " << w_pid << endl;
            }
            if (managed_windows.size() == 0) {
                last_raised_window = NULL;
            }
        }

        // Window management

        void raise_window(Display *dpy, Window w) {
            XRaiseWindow(dpy, w);
            last_raised_window = w;
        }

        int main() {
            if (!(dpy = XOpenDisplay(0x0))) {
                return 1;
            }
            dpy_root = DefaultRootWindow(dpy);
            XSelectInput(dpy, dpy_root, SubstructureNotifyMask);

            this->set_user_events();
            this->set_cursor(XRootWindow(dpy, 0));

            start.subwindow = None;
            for(;;) {
                XNextEvent(dpy, &ev);
                if (ev.type == MapNotify && ev.xmap.window != None) {
                    this->manage_window(ev.xmap.window);
                } else if (ev.type == DestroyNotify && ev.xdestroywindow.window != None) {
                    this->unmanage_window(ev.xdestroywindow.window);
                } else if (ev.type == InputFocus && ev.xfocus.window != None) {
                    this->raise_window(dpy, ev.xfocus.window);
                    cout << "InputFocus event" << endl;
                } else if (ev.type == KeyPress && ev.xkey.subwindow != None) {
                    this->raise_window(dpy, ev.xkey.subwindow);
                } else if(ev.type == ButtonPress && ev.xbutton.subwindow != None) {
                    this->raise_window(dpy, ev.xbutton.subwindow);
                    XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
                    start = ev.xbutton;
                } else if(ev.type == MotionNotify && start.subwindow != None) {
                    int xdiff = ev.xbutton.x_root - start.x_root;
                    int ydiff = ev.xbutton.y_root - start.y_root;
                    XMoveResizeWindow(dpy, start.subwindow,
                        attr.x + (start.button==1 ? xdiff : 0),
                        attr.y + (start.button==1 ? ydiff : 0),
                        MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                        MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
                } else if(ev.type == ButtonRelease) {
                    start.subwindow = None;
                }
            }
        }
    
    private:
        pid_t get_window_pid(Window w) {
            static Atom w_pid;
            if (!w_pid) {
                w_pid = XInternAtom(dpy, "_NET_WM_PID", 0);
                if (w_pid == None) {
                    cout << "Window does not have PID!" << endl;
                    return 0;
                }
            }
            return w_pid;
        }
};

int main() {
    WindowManager wm;
    return wm.main();
}
