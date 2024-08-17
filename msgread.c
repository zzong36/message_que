#include "msglib.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

int main()
{
    int qid;
    struct msgbuf msg;
    key_t key;

    key = ftok("/home/sk/TEST/message/", 'a');
    qid = msgget(key, 0);

    if (msgrcv(qid, &msg, MSG_SZ, 0, IPC_NOWAIT) < 0)
    {
        fprintf(stderr, "msgrcv fail: %s\n", strerror(errno));
        return 0;
    }

    printf("-------------------------------------\n");
    printf("from: %s\n", msg.user_id);
    printf("message : %s\n", msg.mtext);
    printf("-------------------------------------\n");

    return 0;
}
