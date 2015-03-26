#include "utils.h"

#include <time.h>
#include <unistd.h>

namespace utils {

long seedgen() {
    long int seconds;
    return ((time(&seconds)*181)*((getpid() - 83)*359))%104729;
}

}
