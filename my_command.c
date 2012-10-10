#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "my_command.h"

#include "common.h"

// TODO：本文件的测试函数需要删除

/* a signed 16-bit integer */
typedef short int16;
typedef unsigned short uint16;

typedef int int32;
typedef unsigned int uint32;

typedef char char8;
typedef unsigned char uchar8;

typedef char int8;
typedef unsigned char uint8;


typedef unsigned char word;

typedef short BOOL;
typedef short bool;
typedef short Bool;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

typedef struct iblBinBlob_s
{
    uint32   startAddress;          /**< Where the loaded data is placed */
    uint32   sizeBytes;             /**< How much data to load */
    uint32   branchAddress;         /**< Where to branch to when the load is complete */

} iblBinBlob_t;

typedef struct iblEthBootInfo_s
{
    uint8   ipAddr[4];      /**< The IP address of this device */
    uint8   serverIp[4];    /**< The IP address of the tftp server */
    uint8   gatewayIp[4];   /**< The IP address of the gateway */
    uint8   netmask[4];     /**< The IP netmask */
    uint8   hwAddress[6];   /**< The hardware (mac) address of this device */
    char8   fileName[64];  /**< The file name to load */

} iblEthBootInfo_t;

typedef struct iblEth_s
{
    bool     doBootp;           /**< If true a bootp request is generated. If false the @ref iblEthBootInfo_t
                                     table must be populated before the ibl begins execution */
    bool     useBootpServerIp;  /**< If TRUE then the server IP received from the bootp server
                                     is used, if FALSE the one in the ethInfo field is used */
    bool     useBootpFileName;  /**< If TRUE then the file name received from the bootp server
                                     is used, if FALSE the one in the ethInfo field is used */
    int32    bootFormat;        /**< The format of the boot data file. @ref iblBootFormats */

    iblBinBlob_t blob;          /**< Used only if the format is ibl_BOOT_FORMAT_BBLOB */

    iblEthBootInfo_t  ethInfo;  /**< Low level ethernet information */

} iblEth_t;

typedef struct iblBoot_s
{

    int32   bootMode;           /**< Identifies the boot mode @ref iblBootModes */

    uint32  priority;           /**< The boot priority. @ref iblPeriphPriority */
    int32   port;               /**< The port to use, or @ref ibl_PORT_FROM_RBL */

    union  {

        iblEth_t   ethBoot;      /**< Ethernet boot configuration. @ref iblEth_t */

        //iblNand_t  nandBoot;     /**< NAND boot configuration @ref iblNand_t */

        //iblNor_t   norBoot;      /**< NOR boot configuration  @ref iblNor_t */

    } u;

} iblBoot_t;

typedef struct ibl_s
{
    uint32     iblMagic;                      /**< @ref ibl_MAGIC_VALUE */

    //iblPll_t   pllConfig[ibl_N_PLL_CFGS];     /**< PLL Configuration. @ref iblPll_t */

    //iblDdr_t   ddrConfig;                     /**< DDR configuration @ref iblDdr_t  */

    //iblSgmii_t sgmiiConfig[ibl_N_ETH_PORTS];  /**< SGMII boot configuration. @ref iblSgmii_t */

    //iblMdio_t  mdioConfig;                    /**< MDIO configuration. @ref iblMdio_t */

    //iblSpi_t   spiConfig;                     /**< SPI configuration @ref iblSpi_s */

    //iblEmif_t  emifConfig[ibl_MAX_EMIF_PMEM]; /**< EMIF (nand/nor, not ddr) configuration. @ref iblEmif_t */

    iblBoot_t  bootModes[3];   /**< Boot configuration */

    uint16     iblEvmType;                    /**< @ref ibl_EVM_TYPE */

    uint16     chkSum;                        /**< Ones complement checksum over the whole config structure */

} ibl_t;

ibl_t ibl;

#define ibl_BOOT_FORMAT_AUTO    0   /**< Auto determine the boot format from the data */
#define ibl_BOOT_FORMAT_NAME    1   /**< Determines the boot format based on file name (bootp/tftp only) */
#define ibl_BOOT_FORMAT_BIS     2   /**< Boot TI AIS format */
#define ibl_BOOT_FORMAT_COFF    3   /**< Boot a COFF file */
#define ibl_BOOT_FORMAT_ELF     4   /**< Boot an ELF file */
#define ibl_BOOT_FORMAT_BBLOB   5   /**< Boot a binary blob */
#define ibl_BOOT_FORMAT_BTBL    6   /**< Boot a TI boot table file */

#define SETIP(array,i0,i1,i2,i3)      array[0]=(i0);  \
    array[1]=(i1);  \
    array[2]=(i2);  \
    array[3]=(i3)

#define SETMAC(array,i0,i1,i2,i3,i4,i5)      array[0]=(i0);  \
    array[1]=(i1);  \
    array[2]=(i2);  \
    array[3]=(i3);  \
    array[4]=(i4);  \
    array[5]=(i5)

/* just a foolish command table */
cmd_tbl_t cmd_table[] = 
{
    {"help", CONFIG_SYS_MAXARGS, do_help, "print help info."},
    {"print", 1, do_print, "print the env."},
    {"set", 3, do_set, "set env for net boot, type 'set' for more info"},
    {"boot", 1, do_boot, "boot from net"},
    {"exit", 1, do_exit, "exit..."},
    {NULL, 0, NULL, NULL},
};

int do_help(int argc, char * const argv[])
{
    _do_help(cmd_table, sizeof(cmd_table)/sizeof(cmd_tbl_t), argc, argv);
    return 0;
}

void initIBL()
{
    /* TRUE: DHCP  FALSE: not DHCP */
    ibl.bootModes[2].u.ethBoot.doBootp          = FALSE;
    ibl.bootModes[2].u.ethBoot.useBootpServerIp = TRUE;
    ibl.bootModes[2].u.ethBoot.useBootpFileName = TRUE;
    ibl.bootModes[2].u.ethBoot.bootFormat       = ibl_BOOT_FORMAT_AUTO;  // by file data


    SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.ipAddr,    172,18,10,96);
    SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.serverIp,  172,18,10,156);
    SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.gatewayIp, 172,18,10,1);
    SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.netmask,   255,255,0,0);

    /* Use the e-fuse value */
    ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[0] = 0;
    ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[1] = 0;
    ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[2] = 0;
    ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[3] = 0;
    ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[4] = 0;
    ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[5] = 0;


    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[0]  = 'a';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[1]  = 'p';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[2]  = 'p';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[3]  = '.';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[4]  = 'o';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[5]  = 'u';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[6]  = 't';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[7]  = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[8]  = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[9]  = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[10] = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[11] = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[12] = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[13] = '\0';
    ibl.bootModes[2].u.ethBoot.ethInfo.fileName[14] = '\0';
}

int do_print(int argc, char * const argv[])
{
    int bootFormat = -1;
    char* format[] = {
        "AUTO",
        "NAME",
        "BIS",
        "COFF",
        "ELF",
        "BBLOB",
        "BTBL",
    };

    bootFormat = ibl.bootModes[2].u.ethBoot.bootFormat;
    myprintf("dhcp = %s\n", ibl.bootModes[2].u.ethBoot.doBootp ? "TURE" : "FALSE");
    myprintf("bootformat = %s\n", format[bootFormat]);    
    myprintf("bootfile = %s\n", ibl.bootModes[2].u.ethBoot.ethInfo.fileName);
    myprintf("serverip = %d.%d.%d.%d\n", 
        ibl.bootModes[2].u.ethBoot.ethInfo.serverIp[0],
        ibl.bootModes[2].u.ethBoot.ethInfo.serverIp[1],
        ibl.bootModes[2].u.ethBoot.ethInfo.serverIp[2],
        ibl.bootModes[2].u.ethBoot.ethInfo.serverIp[3]);
    myprintf("ipaddr = %d.%d.%d.%d\n",
        ibl.bootModes[2].u.ethBoot.ethInfo.ipAddr[0],
        ibl.bootModes[2].u.ethBoot.ethInfo.ipAddr[1],
        ibl.bootModes[2].u.ethBoot.ethInfo.ipAddr[2],
        ibl.bootModes[2].u.ethBoot.ethInfo.ipAddr[3]);
    myprintf("gateway = %d.%d.%d.%d\n",
        ibl.bootModes[2].u.ethBoot.ethInfo.gatewayIp[0],
        ibl.bootModes[2].u.ethBoot.ethInfo.gatewayIp[1],
        ibl.bootModes[2].u.ethBoot.ethInfo.gatewayIp[2],
        ibl.bootModes[2].u.ethBoot.ethInfo.gatewayIp[3]);
    myprintf("netmask = %d.%d.%d.%d\n",
        ibl.bootModes[2].u.ethBoot.ethInfo.netmask[0],
        ibl.bootModes[2].u.ethBoot.ethInfo.netmask[1],
        ibl.bootModes[2].u.ethBoot.ethInfo.netmask[2],
        ibl.bootModes[2].u.ethBoot.ethInfo.netmask[3]);
    myprintf("ethaddr = %02x:%02x:%02x:%02x:%02x:%02x\n",
        ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[0],
        ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[1],
        ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[2],
        ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[3],
        ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[4],
        ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress[5]);
    return 0;
}

static void do_set_usage()
{
    myprintf("set bootfile [boot file].\n");
    myprintf("set format   [0(AUTO)|1(NAME)|2(BIS)|3(COFF)|4(ELF)|5(BBLOB)]|[6(BTBL)].\n");
    myprintf("set serverip [server ip].\n");
    myprintf("set ipaddr   [board ip].\n");
    myprintf("set gateway  [gateway].\n");
    myprintf("set netmask  [netmask].\n");
    myprintf("set ethaddr  [mac addr].\n");
    myprintf("set dhcp     [0|1].\n\n");
}

static void getaddr(char* src, char* dst, int len, int radix)
{
    char* end;
    int i;

    for (i = 0; i < len; ++i) {
        dst[i] = src ? simple_strtoul(src, &end, radix) : 0;
        if (src)
            src = (*end) ? end + 1 : end;
    }
    //*++dst = '\0';
}

int do_set(int argc, char * const argv[])
{ 
    char tmp[6] = {0};
    int dhcp = 0;
    int format = 0;

    if (argc != 3)
    {
        myprintf("Invalid argument. Usage:\n\n");
        do_set_usage();
        return -1;
    }
    if (!strcmp("bootfile", argv[1]))
    {
        myprintf("bootfile\n");
        myprintf("file: %s\n", argv[2]);
        strcpy(ibl.bootModes[2].u.ethBoot.ethInfo.fileName, argv[2]);
    } 
    else if (!strcmp("format", argv[1]))
    {
        myprintf("format\n");
        myprintf("format: %s\n", argv[2]);
        format = atoi(argv[2]);
        if (format < 0 || format > 6)
        {
            myprintf("Only 0~6 support.\n");
            return -1;
        }
        ibl.bootModes[2].u.ethBoot.bootFormat = format;
    }
    else if (!strcmp("serverip", argv[1]))
    {
        myprintf("serverip\n");
        myprintf("serverip: %s\n", argv[2]);
        getaddr(argv[2], tmp, 4, 10);
        SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.serverIp, tmp[0],tmp[1],tmp[2],tmp[3]);

    }
    else if (!strcmp("ipaddr", argv[1]))
    {
        myprintf("ipaddr\n");
        myprintf("ipaddr: %s\n", argv[2]);
        getaddr(argv[2], tmp, 4, 10);
        SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.ipAddr, tmp[0],tmp[1],tmp[2],tmp[3]);
    }
    else if (!strcmp("gateway", argv[1]))
    {
        myprintf("gateway\n");
        myprintf("gateway: %s\n", argv[2]);
        getaddr(argv[2], tmp, 4, 10);
        SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.gatewayIp, tmp[0],tmp[1],tmp[2],tmp[3]);
    }
    else if (!strcmp("netmask", argv[1]))
    {
        myprintf("netmask\n");
        myprintf("netmask: %s\n", argv[2]);
        getaddr(argv[2], tmp, 4, 10);
        SETIP(ibl.bootModes[2].u.ethBoot.ethInfo.netmask, tmp[0],tmp[1],tmp[2],tmp[3]);
    }
    else if (!strcmp("ethaddr", argv[1]))
    {
        myprintf("ethaddr\n");
        myprintf("ethaddr: %s\n", argv[2]);
        getaddr(argv[2], tmp, 6, 16);
        SETMAC(ibl.bootModes[2].u.ethBoot.ethInfo.hwAddress, tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5]);
    }
    else if (!strcmp("dhcp", argv[1]))
    {
        myprintf("dhcp\n");
        dhcp = atoi(argv[2]);
        myprintf("dhcp: %s %d\n", argv[2], dhcp);
        if (dhcp != 0)
        {
            myprintf("dhcp TRUE\n");
            ibl.bootModes[2].u.ethBoot.doBootp = TRUE;
        }
        else
        {
            myprintf("dhcp FALSE\n");
            ibl.bootModes[2].u.ethBoot.doBootp = FALSE;
        }
    }
    else
    {
        myprintf("Unsupport command. Usage:\n\n");
        do_set_usage();
        return -1;
    }

    return 0;
}

int do_boot(int argc, char * const argv[])
{
    return 0;
}

int do_exit(int argc, char * const argv[])
{
    exit(0);
}