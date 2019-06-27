#include "lib.h"

struct {
        char msg1[5000];
        char msg2[1000];
} data = {
        "this is initialized data",
        "so is this"
};

char bss[6000];

inline int MY_MUL(int a,int b)
{
        int i;
        int sum;
        for(i=0,sum=0;i<b;sum+=a,i++);
        return sum;
}

int
sum(char *s, int n)
{
        int i, tot;

        tot = 0;
        for(i=0; i<n; i++)
                tot ^= MY_MUL(i,s[i]);
        return tot;
}

void
umain(int argc, char **argv)
{
        int i, r, x, want;

        writef("init: runningn");

        want = 0xf989e;
        if ((x=sum((char*)&data, sizeof data)) != want)
                writef("init: data is not initialized: got sum %08x wanted %08xn",
                        x, want);
        else
                writef("init: data seems okayn");
        if ((x=sum(bss, sizeof bss)) != 0)
                writef("bss is not initialized: wanted sum 0 got %08xn", x);
        else
                writef("init: bss seems okayn");

        writef("init: args:");
        for (i=0; i<argc; i++)
                writef(" '%s'", argv[i]);
        writef("n");

        writef("init: running shn");

        // being run directly from kernel, so no file descriptors open yet
        close(0);
        if ((r = opencons()) < 0)
                user_panic("opencons: %e", r);
        if (r != 0)
                user_panic("first opencons used fd %d", r);
        if ((r = dup(0, 1)) < 0)
                user_panic("dup: %d", r);

write(1,"LALA",4);

        for (;;) {
                writef("init: starting shn");
                r = spawnl("sh.b", "sh", (char*)0);
                if (r < 0) {
                        writef("init: spawn sh: %en", r);
                        continue;
                }
                wait(r);

        }
}

