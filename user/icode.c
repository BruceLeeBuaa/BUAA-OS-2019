#include "lib.h"

void
umain(void)
{
        int fd, n, r;
        char buf[512+1];

        writef("icode: open /motdn");
        if ((fd = open("/motd", O_RDONLY)) < 0)
                user_panic("icode: open /motd: %e", fd);

        writef("icode: read /motdn");
        while ((n = read(fd, buf, sizeof buf-1)) > 0){
                buf[n] = 0;
                writef("%sn",buf);
        }

        writef("icode: close /motdn");
//      close(fd);

        writef("icode: spawn /initn");
        if ((r = spawnl("init.b", "init", "initarg1", "initarg2", (char*)0)) < 0)
                user_panic("icode: spawn /init: %e", r);

        writef("icode: exitingn");
}

