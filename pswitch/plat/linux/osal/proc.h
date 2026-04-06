#ifndef __LINUX_PROC_H__
#define __LINUX_PROC_H__

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>

typedef struct proc_dir_entry osal_proc_entry;

#ifdef CONFIG_SYS_KERNEL_LINUX_3_18_24_X
#define PROC_SEQ_CREATE(name, optsName)                           \
do {                                                              \
    osal_proc_entry *ent;                                         \
    ent = proc_create(name, S_IRUGO, NULL, &proc_ops_##optsName); \
} while(0)

#define PROC_SEQDIR_CREATE(dirname, pDir, name, optsName)         \
do {                                                              \
    osal_proc_entry *ent;                                         \
    if (NULL == pDir)                                             \
        pDir = proc_mkdir(dirname, NULL);                         \
    ent = proc_create(name, S_IRUGO, pDir, &proc_ops_##optsName); \
} while(0)
#else
#define PROC_SEQ_CREATE(name, optsName)                   \
do {                                                      \
    osal_proc_entry *ent;                                 \
    ent = create_proc_entry(name, S_IRUGO, NULL);         \
    if (ent) ent->proc_fops = &proc_ops_##optsName;       \
} while(0)

#define PROC_SEQDIR_CREATE(dirname, pDir, name, optsName) \
do {                                                      \
    osal_proc_entry *ent;                                 \
    if (NULL == pDir)                                     \
        pDir = proc_mkdir(dirname, NULL);                 \
    ent = create_proc_entry(name, S_IRUGO, pDir);         \
    if (ent) ent->proc_fops = &proc_ops_##optsName;       \
} while(0)
#endif

#define PROC_REMOVE(name)                                 \
do {                                                      \
    remove_proc_entry(name, NULL);                        \
} while(0)

#define PROC_SEQ_PRINTF(_buf, _buflen, _pLen, a, args...) \
do {                                                      \
    *_pLen += sprintf((_buf + *_pLen), a, ##args);        \
    if (*_pLen > _buflen)                                 \
        return -1;                                        \
} while(0)

#define PROC_SEQ_INIT(name, bufsize, dumpcb)                            \
static char *p_procSeq_##name = NULL;                                   \
static uint32 g_procSecLen_##name = 0;                                  \
static void* seq_start_##name(struct seq_file *s, loff_t *pos)          \
{                                                                       \
    if (*pos != 0)                                                      \
    {                                                                   \
        if (*pos >= g_procSecLen_##name)                                \
        {                                                               \
            *pos = 0;                                                   \
            osal_free(p_procSeq_##name);                                \
            p_procSeq_##name = NULL;                                    \
            return NULL;                                                \
        }                                                               \
        else                                                            \
        {                                                               \
            return (p_procSeq_##name + *pos);                           \
        }                                                               \
    }                                                                   \
    p_procSeq_##name = osal_alloc(bufsize);                             \
    if (!p_procSeq_##name)                                              \
        return NULL;                                                    \
    g_procSecLen_##name = 0;                                            \
    dumpcb(p_procSeq_##name, bufsize, &g_procSecLen_##name);            \
    return p_procSeq_##name;                                            \
}                                                                       \
static void* seq_next_##name(struct seq_file *s, void *v, loff_t *pos)  \
{                                                                       \
    *pos = *pos + 1;                                                    \
    if ((*pos >= g_procSecLen_##name) || (*pos >= bufsize))             \
    {                                                                   \
        *pos = 0;                                                       \
        return NULL;                                                    \
    }                                                                   \
    return (v + 1);                                                     \
}                                                                       \
static void seq_stop_##name(struct seq_file *s, void *v)                \
{                                                                       \
}                                                                       \
static int seq_show_##name(struct seq_file *s, void *v)                 \
{                                                                       \
    seq_putc(s, *(char *)v);                                            \
    return 0;                                                           \
}                                                                       \
static struct seq_operations seq_ops_##name = {                         \
    .start = seq_start_##name,                                          \
    .next  = seq_next_##name,                                           \
    .stop  = seq_stop_##name,                                           \
    .show  = seq_show_##name                                            \
};                                                                      \
static int proc_open_##name(struct inode *inode, struct file *file)     \
{                                                                       \
    return seq_open(file, &seq_ops_##name);                             \
}                                                                       \
static struct file_operations proc_ops_##name = {                       \
    .owner   = THIS_MODULE,                                             \
    .open    = proc_open_##name,                                        \
    .read    = seq_read,                                                \
    .llseek  = seq_lseek,                                               \
    .release = seq_release                                              \
}

#endif 

