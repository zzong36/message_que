#include "msglib.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* 변수 선언 */
    int qid, st;
    key_t key;
    msgbuf msg;
    struct msqid_ds msqid;
    long pid;

    /* 아이디 입력 여부 확인 */
    if (argc < 2)
    {
        printf("need id\n");
        printf("ex) ./msgclient [id]\n");
        return 0;
    }

    /* IPC key 생성 */
    key = ftok("/home/TEST/message/", 'a');

    /* 메세지 큐 얻어오기 */
    qid = msgget(key, 0);

    /* pid 저장 */
    pid = getpid();

    while (1)
    {
        /* 메세지 세팅 */
        memset(msg.mtext, '\0', sizeof(msg.mtext));
        printf("input : ");
        fgets(msg.mtext, sizeof(msg.mtext), stdin);
        msg.mtype = 1L;
        msg.from_pid = pid;
        strncpy(msg.user_id, argv[1], sizeof(msg.user_id) - 1);
        msg.user_id[sizeof(msg.user_id) - 1] = '\0';

        /* fgets 함수 줄바꿈 제거 */
        size_t len = strlen(msg.mtext);
        if (msg.mtext[len - 1] == '\n')
            msg.mtext[len - 1] = '\0';

        /* 메세지 송신 */
        if (msgsnd(qid, &msg, MSG_SZ, IPC_NOWAIT) < 0)
        {
            fprintf(stderr, "msgsnd fail : %s\n", strerror(errno));
            break;
        }
        /* 채팅방 종료할 경우 */
        if (strcmp(msg.mtext, "exit") == 0)
        {
            printf("chat terminated\n");
            sleep(1);
            msgrcv(qid, &msg, MSG_SZ, pid, 0);
            break;
        }

        /* 서버로부터 전송여부 수신 */
        st = msgrcv(qid, &msg, MSG_SZ, pid, 0);
        if (st < 0)
        {
            fprintf(stderr, "msgrcv fail : %s\n", strerror(errno));
            break;
        }

        /* 서버가 보낸 메세지 출력 */
        if (st > 0)
            printf("%s\n", msg.mtext);
    }
    return 0;
}
