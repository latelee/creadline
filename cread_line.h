/**
 * @file   cread_line.h
 * @author Late Lee 
 * @date   2012-9-5 22:52:20
 * @brief
 *
 *      �ο�u-boot��creadlineʵ�֣��������£�
        1��֧��Linux��readline�󲿷ֹ��ܣ��������£�
        0)���������ҷ����
        1)��c-a: �Ƶ���������
        2)��c-e: �Ƶ�������β
        3)��c-f: ǰ��һ�ַ�
        4)��c-b: ����һ�ַ�
        5)��c-x/c-u: ɾ������
        6)��c-d: ɾ����굱ǰ�ַ�
        7)��c-k: ɾ����ǰ�������β�ַ�
        8)��c-p: ǰһ����ʷ����
        9)��c-n: ��һ����ʷ����
        10)��HOME/END: ��������/��β
        11)��Delete��Windows-ɾ����ǰ����ַ� Linux-ɾ�����ǰ�ַ�
        
        2����������л�����Ϊ256�ַ�
        3��֧����ʷ������10����¼��
        4��֧�ֶ��������������룬�Էֺš�;�����ӡ�

        ������
        δ��ɹ��ܣ�
        Ctrl-u������readline�ⲻ��ͬ
        Ctrl-y����δ���


        Delte������
        Windows��ɾ����ǰ��������ַ�
        Linux  ��ɾ����ǰ���ǰһ���ַ�
 *
 */

#ifndef CREAD_LINE_H
#define CREAD_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

//#define SIMPLE_READLINE   /* very simple one using gets() */

#define PROMPT          "NotAShell> "
#define CB_SIZE         256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

/* history command BEGIN */
#define HIST_MAX		10
#define HIST_SIZE		MAX_CMDBUF_SIZE

int readline (const char *const prompt, char* line_buf);

#ifdef __cplusplus
};
#endif

#endif /* CREAD_LINE_H */
