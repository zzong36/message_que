#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>

#define MSG_SZ sizeof(msgbuf)-sizeof(long)

typedef struct msgbuf{
        long mtype;
        char mtext[512];
        long from_pid;
        char user_id[20];
} msgbuf;

void _CHK_MSQID(int qid, struct msqid_ds msqid);
void _CHK_MSGBUF(msgbuf msg);
void _RCV_MSG(msgbuf msg);
void _RM_CRG(msgbuf msg);
bool contains(long* array, size_t size, long value);
long* add_value(long* array, size_t* size, long value);
long* remove_value(long* array, size_t *size, long value);
void print_userlist(long * array, size_t size);
