# 交互式命令终端实现


# 功能
模拟一般的命令行终端的使用。  

命令长度为256，由CB_SIZE指定。  
命令行提示符由 PROMPT 指定。  
支持历史命令，使用向上/下箭头切换，由HIST_MAX指定数量，默认10个。  
开启 CMD_LITE 使用精简版本，较多功能不支持。  

