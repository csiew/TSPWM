#include <iostream>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xcursor/Xcursor.h>

using namespace std;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define XC_left_ptr 68

class WindowManager {
    public:
        Display * dpy;
        XWindowAttributes attr;
        XButtonEvent start;
        XEvent ev;

        void set_cursor(Window w) {
            Cursor c = XCreateFontCursor(dpy, XC_left_ptr);
            XDefineCursor(dpy, w, c);
            XFlush(dpy);
        }

        int main() {
            if (!(dpy = XOpenDisplay(0x0))) {
                return 1;
            }

            XGrabKey(
                dpy,
                XKeysymToKeycode(dpy, XStringToKeysym("F1")),
                Mod1Mask|Mod2Mask,
                DefaultRootWindow(dpy),
                True,
                GrabModeAsync,
                GrabModeAsync
            );
            XGrabButton(
                dpy,
                1,
                Mod1Mask|Mod2Mask,
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
                Mod1Mask|Mod2Mask,
                DefaultRootWindow(dpy),
                True,
                ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
                GrabModeAsync,
                GrabModeAsync,
                None,
                None
            );

            this->set_cursor(XRootWindow(dpy, 0));

            start.subwindow = None;
            for(;;) {
                XNextEvent(dpy, &ev);
                if (ev.type == CreateNotify) {
                    this->set_cursor(ev.xany.window);
                }
                if (ev.type == KeyPress && ev.xkey.subwindow != None) {
                    XRaiseWindow(dpy, ev.xkey.subwindow);
                } else if(ev.type == ButtonPress && ev.xbutton.subwindow != None) {
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
};

int main() {
    WindowManager wm;
    return wm.main();
}
