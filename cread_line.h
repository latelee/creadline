/**
 * @file   cread_line.h
 * @author Late Lee 
 * @date   2012-9-5 22:52:20
 * @brief
 *
 *      �ο�u-boot��creadlineʵ�֣��������£�
        1��֧��Linux��readline�󲿷ֹ��ܣ�
        1)��c-a:
        2)��c-e:
        2��֧����ʷ����ݶ����20����¼��
        3��֧�ֶ��������������룬�Էֺš�;�����ӡ�

        δ��ɹ��ܣ�
        Ctrl-u����δ��� Ctrl-y����δ���


        Delte������
        Windows��ɾ����ǰ��������ַ�
        Linux  ��ɾ����ǰ���ǰһ���ַ�
 *
 */

#ifndef CREAD_LINE_H
#define CREAD_LINE_H

//#define SIMPLE_READLINE   /* very simple one using gets() */

#define PROMPT          "NotAShell> "
#define CB_SIZE         256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

/* history command BEGIN */
#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

int readline (const char *const prompt, char* line_buf);

#endif /* CREAD_LINE_H */
