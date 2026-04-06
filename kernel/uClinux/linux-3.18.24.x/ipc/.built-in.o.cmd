cmd_ipc/built-in.o :=  rtk-ms-2.0.0-linux-ld  -m elf32btsmip   -r -o ipc/built-in.o ipc/util.o ipc/msgutil.o ipc/msg.o ipc/sem.o ipc/shm.o ipc/ipcns_notifier.o ipc/syscall.o ipc/ipc_sysctl.o 
