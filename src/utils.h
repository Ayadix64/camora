#ifndef UTILS
#define UTILS

#include "../ticktackto/include/tick-tack-to.h"
#include <string>


#define min(x,y) x<y?x:y
#define max(x,y) x>y?x:y
#define pos_or_zeor(x) x<0?0:x
#define cube(x) (x)*(x) // yeah..
#define CONFIG_FILE "camora.conf"




#ifndef MAIN
extern "C" struct {
        u32 defCamera = 0;
        float dim = 1.0;
        float animationSpeed = 1.0;
        std::string pictorsLocation = "./";
} parameters;
#endif

#endif
