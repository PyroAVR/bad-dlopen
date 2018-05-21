#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const char help_msg[] = "Usage: %s <module>\n";

int main(int argc, char **argv) {

    //poor error handling, at best
    if(argc < 2)    {
        fprintf(stderr, help_msg, argv[0]);
        exit(EXIT_FAILURE);
    }

    //ask the OS for a file descriptor - like a file, but without any nice
    //wrappers. More powerful, also more difficult.
    int module_fd = open(argv[1], O_RDONLY);
    if(module_fd == -1)  {
        fprintf(stderr, "could not open %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    //get size of the file (just read the whole thing for now)
    off_t end   = lseek(module_fd, 0, SEEK_END);
    off_t start = lseek(module_fd, 0, SEEK_SET);
    int size    = end-start;
    printf("mapped file size: %d\n",size);
    //mmap and set execute bit
    char *module = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE, module_fd, 0);
    if(module == MAP_FAILED)    {
        fprintf(stderr, "could not map: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    
    //get the address of our function - from readelf -s /objdump -s
    char *five = module + 0x40;
    int x = ((int (*)(void))five)();
    printf("the value of x is %d\n", x);

    //same deal, try to pass an arg... doesn't work, likely due to calling
    //convention.
    /*
     *char *fib = module + 0x40 + 0xb;
     *int f5    = ((int (*)(int))fib)(5);
     *printf("the fifth fibonacci number is: %d\n", f5);
     */

    munmap(module, size);
    close(module_fd);
    return 0; 
}
