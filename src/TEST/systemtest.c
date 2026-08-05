#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main() {
    if (chdir("..") != 0) {
        perror("chdir");
        return 1;
    }
    return 0;
}
