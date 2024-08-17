#include "msglib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
        /* 변수 선언 */
        key_t key;
        int qid, st;
        msgbuf msg;
        struct msqid_ds msqid;
        long *user_list = (long *)malloc(5 * sizeof(long));
        size_t size = 0;

        /* IPC 키 생성 */
        key = ftok("/home/TEST/message/", 'a');

        /* 메세지 큐 생성 */
        qid = msgget(key, IPC_CREAT | 0777);
        printf("msgserver START\n");
        printf("----------------------------\n");
        while (1)
        {
                /* 메세지 수신 */
                st = msgrcv(qid, &msg, MSG_SZ, 1L, 0);

                /* 클라이언트의 PID 이용자 목록에 등록(중복등록X)*/
                if (!contains(user_list, size, msg.from_pid))
                {
                        user_list = add_value(user_list, &size, msg.from_pid);
                }

                /* 클라이언트가 보낸 메세지 유형과 서버가 보내는 메세지 유형 분리  */
                if (msg.mtype == 1L)
                {
                        /* 클라이언트가 종료 메세지를 보낼 경우 */
                        if (!strcmp(msg.mtext, "exit"))
                        {
                                printf("%s left the chat\n", msg.user_id);
                                /* 이용자 목록에서 클라이언트 제거 */
                                remove_value(user_list, &size, msg.from_pid);
                        }
                        else
                        {
                                /* 클라이언트 메세지 print */
                                _RCV_MSG(msg);
                        }
                        /* 클라이언트에게 보낸 메세지 세팅 */
                        msg.mtype = msg.from_pid;
                        strncpy(msg.mtext, "message sent", sizeof(msg.mtext));
                        strncpy(msg.user_id, "server", sizeof(msg.user_id));

                        /* 클라이언트에게 메세지 전송 */
                        st = msgsnd(qid, &msg, MSG_SZ, IPC_NOWAIT);
                        if (st < 0)
                        {
                                printf("server message sent fail\n");
                                break;
                        }

                        /* 채팅방에 이용자 모두 떠나면 프로그램 종료*/
                        if (size == 0)
                        {
                                printf("closing chat room\n");
                                break;
                        }
                }
        }
        /* 메세지 큐 제거 */
        msgctl(qid, IPC_RMID, 0);
        return 0;
}
