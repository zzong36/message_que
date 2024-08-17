#include "msglib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void _CHK_MSQID(int qid, struct msqid_ds msqid)
{
    msgctl(qid, IPC_STAT, &msqid);
    printf("msg_stime : %ld\n", msqid.msg_stime);
    printf("msg_rtime : %ld\n", msqid.msg_rtime);
    printf("msg_ctime : %ld\n", msqid.msg_ctime);
    printf("msg_qnum : %ld\n", msqid.msg_qnum);
    printf("msg_qbytes : %ld\n", msqid.msg_qbytes);
    printf("msg_lspid : %d\n", msqid.msg_lspid);
    printf("msg_rspid : %d\n", msqid.msg_lrpid);
}

void _CHK_MSGBUF(msgbuf msg)
{
    printf("mtype(type of message) : %ld\n", msg.mtype);
    printf("mtext(message) : %s\n", msg.mtext);
    printf("from_id(pid that sent the message) : %ld\n", msg.from_pid);
    printf("user_id : %s\n", msg.user_id);
}

void _RCV_MSG(msgbuf msg)
{
    printf("%s: %s\n", msg.user_id, msg.mtext);
}

void _RM_CRG(msgbuf msg)
{
    int len = strlen(msg.mtext);

    if (msg.mtext[len - 1] == '\n')
    {
        msg.mtext[len - 1] = '\0';
    }
}

bool contains(long *array, size_t size, long value)
{
    int i;
    for (i = 0; i < size; ++i)
    {
        if (array[i] == value)
        {
            return true;
        }
    }
    return false;
}

long *add_value(long *array, size_t *size, long value)
{
    size_t new_size = *size + 1;
    long *new_array = (long *)realloc(array, new_size * sizeof(long));

    if (new_array == NULL)
    {
        perror("Failed to allocate memory");

        exit(EXIT_FAILURE);
    }

    new_array[*size] = value;
    *size = new_size;
    return new_array;
}

long *remove_value(long *array, size_t *size, long value)
{
    size_t i;
    for (i = 0; i < *size; ++i)
    {
        if (array[i] == value)
        {
            break;
        }
    }

    if (i == *size)
    {
        return array;
    }

    for (size_t j = i; j < *size - 1; ++j)
    {
        array[j] = array[j + 1];
    }

    size_t new_size = *size - 1;
    long *new_array = (long *)realloc(array, new_size * sizeof(long));
    if (new_array == NULL && new_size > 0)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    *size = new_size;
    return new_array;
}

void print_userlist(long *array, size_t size)
{
    int i;
    for (i = 0; i < size; ++i)
    {
        printf("%ld ", array[i]);
    }
    printf("\n");

    return;
}
