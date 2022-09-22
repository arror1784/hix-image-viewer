#include "singleton.h"

#include <fstream>
#include <filesystem>
#include <iostream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>


#define  KEY_NUM   1152
#define  MEM_SIZE  100000

typedef struct
{
    long msg_type;
    char mtext[256];
}messageBuf;

class Communicate : public Singleton<Communicate>{

public:
    explicit Communicate(){
        _mqKey = msgget(115200,IPC_CREAT|0666);

        if (_mqKey == -1){
            perror("msgget error : ");
        }
        _shmid = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666);
        if(_shmid == -1) {
            std::cout << "shm open fail" << std::endl;

        }else{
            std::cout << "open sucess" << std::endl;

        }
        void *shmaddr;
        shmaddr = shmat(_shmid, (void *)0, 0);
        if((_addr) == (void *)-1){
            perror("Shmat failed");
            return;

        }
        _addr = (uint8_t*)shmaddr;
    }
    void addData(uint8_t *data,int size){
        // std::copy_n(data,size,ummap.begin());
        SharedMemoryWrite(data,size);
        std::string msg = "{\"cmd\":\"imageChanged\",\"size\":" + std::to_string(size) + "}";

        sendMessage(msg);
    }
    void getData(uint8_t *data,int size){
        SharedMemoryRead(data,size);
    }
    bool receiveMessage(messageBuf* buf,bool blocking){
        if(msgrcv( _mqKey, buf, sizeof(messageBuf), 1, blocking ? 0 : IPC_NOWAIT) == -1)
            return false;
        return true;
    }
    uint8_t* _addr = NULL;

private:
    void sendMessage(std::string msg){
        messageBuf mybuf;
        if(msg.length() > 255){
            return;
        }
        std::fill_n(mybuf.mtext,256,0x00);
        std::copy_n(msg.data(),msg.length(),mybuf.mtext);
        mybuf.msg_type = 1;
        if (msgsnd( _mqKey, &mybuf, sizeof(messageBuf), IPC_NOWAIT) == -1)
        {
            perror("msgsnd error : ");
            // exit(0);
        }   
    }
    bool SharedMemoryWrite(uint8_t *shareddata, int size){
        if(!_addr)
            return false;
        if(size > MEM_SIZE){
            printf("Shared memory size over");
            return false;
        }
        memcpy(_addr, shareddata, size);
        
        return true;
    }
    bool SharedMemoryRead(uint8_t *sMemory,int size){
        if(!_addr)
            return false;
        
        if(size > MEM_SIZE)
            return false;

        memcpy(sMemory, (char *)_addr, size);

        return true;
    }
    bool SharedMemoryFree(void){
        if(!_addr)
            return false;
        if(shmctl(_shmid, IPC_RMID, 0) == -1){
            perror("Shmctl failed");
            return false;
        }
    }
    std::error_code error;

    key_t _mqKey;
    int _shmid;

};