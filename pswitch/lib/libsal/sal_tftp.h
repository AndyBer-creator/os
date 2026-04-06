
#ifndef __SAL_TFTP_H__
#define __SAL_TFTP_H__

#define SYS_TFTP_ERR_FILE "/var/tftperr"

typedef enum sys_tftpFileType_e
{
    SYS_TFTP_FILETYPE_IMAGE = 0,
    SYS_TFTP_FILETYPE_CONFIG,
    SYS_TFTP_FILETYPE_END
} sys_tftpFileType_t;

typedef enum sys_tftpOpType_e
{
    SYS_TFTP_OPTYPE_DOWNLOAD = 0,
    SYS_TFTP_OPTYPE_UPLOAD,
    SYS_TFTP_OPTYPE_END
} sys_tftpOpType_t;

extern int32 sal_tftp_fwImage_download(char *lFilename, char *rFilename, char *host);

extern int32 sal_tftp_fwImage_upload(uint32 partition, char *rFilename, char *host);

extern int32 sal_tftp_config_download(char *lFilename, char *rFilename, char *host);

extern int32 sal_tftp_config_upload(char *lFilename, char *rFilename, char *host);

#endif 

