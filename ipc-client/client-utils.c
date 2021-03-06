
#include "client-utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

// inspired by dwm's gettextprop()
GString* window_property_to_g_string(Display* dpy, Window window, Atom atom) {
    GString* result = NULL;
    char** list = NULL;
    int n = 0;
    XTextProperty prop;

    if (0 == XGetTextProperty(dpy, window, &prop, atom)) {
        return NULL;
    }
    // convert text property to a gstring
    if (prop.encoding == XA_STRING
        || prop.encoding == XInternAtom(dpy, "UTF8_STRING", False)) {
        result = g_string_new((char*)prop.value);
    } else {
        if (XmbTextPropertyToTextList(dpy, &prop, &list, &n) >= Success
            && n > 0 && *list)
        {
            result = g_string_new(*list);
            XFreeStringList(list);
        }
    }
    XFree(prop.value);
    return result;
}

// duplicates an argument-vector
char** argv_duplicate(int argc, char** argv) {
    if (argc <= 0) return NULL;
    char** new_argv = malloc(sizeof(char*) * argc);
    if (!new_argv) {
        die("cannot malloc - there is no memory available\n");
    }
    int i;
    for (i = 0; i < argc; i++) {
        new_argv[i] = g_strdup(argv[i]);
    }
    return new_argv;
}

// frees all entries in argument-vector and then the vector itself
void argv_free(int argc, char** argv) {
    if (argc <= 0) return;
    int i;
    for (i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void die(const char *errstr, ...) {
    va_list ap;
    va_start(ap, errstr);
    vfprintf(stderr, errstr, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}
