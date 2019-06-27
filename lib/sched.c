#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
void sched_yield(void)
{
        static pos = 0;
        static times = 0;
        static struct Env *e = NULL;
        if(e!=NULL && e->env_status != ENV_RUNNABLE){
                LIST_REMOVE(e,env_sched_link);
                e = NULL;
                times = 0;
        }
        if(times == 0) {
                if(LIST_FIRST(&env_sched_list[pos]) == NULL){
                        pos = 1 - pos;
                }
                e = LIST_FIRST(&env_sched_list[pos]);
                times = e->env_pri;
                LIST_REMOVE(e,env_sched_link);
                LIST_INSERT_HEAD(&env_sched_list[1-pos],e,env_sched_link);
        }
        times = times - 1;
        env_run(e);
}

