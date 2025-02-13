#include "includes.h"

const char *VERSION = "1.0.0";
volatile int done = 0;

const char *frames[] = {
    "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"
};
const int nframes = sizeof(frames) / sizeof(frames[0]);
