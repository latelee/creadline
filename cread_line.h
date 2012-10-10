/**
 * @file   cread_line.h
 * @author Late Lee 
 * @date   2012-9-5 22:52:20
 * @brief
 *
 *      �ο�u-boot��creadlineʵ�֣��������£�
        1��֧��Linux��readline�󲿷ֹ��ܣ���������(c:��ʾ��Ctrl����c-a��ͬʱCtrl��a)��
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
        10)��c-c���ж�����
        11)��HOME/END: ��������/��β
        12)��Delete��Windows-ɾ����ǰ����ַ� Linux-ɾ�����ǰһ�ַ�(��Linux����)
        13)��Backspace��ɾ�����ǰһ�ַ�
        ����һЩת���ַ���
        14)��c-h����ֵΪ0x8����Backspace����
        15)��c-i����ֵΪ0x9����tab����Linux�¿��ܻ��ǲ��������
        16)��c-j����ֵΪ0xa������  c-m����ֵΪ0xd���س�   ע����Linux��Windows�У����߱�����ͬ��
        
        2����������л�����Ϊ256�ַ�
        3��֧����ʷ������10����¼��
        4��֧�ֶ��������������룬�Էֺš�;�����ӡ�

        ������
        δ��ɹ��ܣ�
        Ctrl-u������readline�ⲻ��ͬ
        Ctrl-y����δ���


        Delte������
        Windows��ɾ����ǰ��������ַ�(����Linux����)
        Linux  ��ɾ����ǰ���ǰһ���ַ�
 *
 * log
       2012-09-18
       ���CMD_LITE�꣬����ú�ʱ�ɽ�ʡ�ڴ棬����֧��Backspace��Delete����
 *
 * note
       2012-09-26
       �������������
       Windows: 
                up:     0xe0 'H'
                down:   0xe0 'P'
                right:  0xe0 'M'
                left:   0xe0 'K'

                HOME:   0xe0 'G'
                END:    0xe0 'O'(��д��ĸO)
                Delete: 0xe0 'S'
        Linux��
                up:     0x1b 0x5b 0x41  ^[[A
                down:   0x1b 0x5b 0x42  ^[[B
                right:  0x1b 0x5b 0x43  ^[[C
                left:   0x1b 0x5b 0x44  ^[[D
                
                HOME:   ?
                END:    ? 
                Delete: 0x7f 
 */

#ifndef CREAD_LINE_H
#define CREAD_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

//#define SIMPLE_READLINE   /* very simple one using gets() */
//#define CMD_LITE    /* define it to save text and memory */

#define PROMPT          "NotAShell> "
#define CB_SIZE         256 /* console buffer size */
#define MAX_CMDBUF_SIZE CB_SIZE

#define HIST_MAX        10              /* history cmd number */
#define HIST_SIZE       MAX_CMDBUF_SIZE /* history cmd buffer size */

int readline (const char *const prompt, char* line_buf);

#ifdef __cplusplus
};
#endif

#endif /* CREAD_LINE_H */
