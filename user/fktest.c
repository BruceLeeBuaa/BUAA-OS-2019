#include "lib.h"


void umain()
{
        int a=0;
        int id=0;

        if((id=fork())==0)
        {
                if ((id=fork())==0)
                {
                        a+=3;
                        for(;;) writef("ttthis is child2 :a:%dn",a);
                }
                a+=2;
                for(;;) writef("tthis is child :a:%dn",a);
        }
        a++;
        for(;;) writef("this is father: a:%dn",a);
}

