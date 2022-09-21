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

class Communicate : public Singleton<Communicate>{

    struct messageBuf
    {
        long msg_type;
        char mtext[256];
    };

public:
    explicit Communicate(){
        _shmid = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666);
        if(_shmid == -1) {
            std::cout << "shm open fail" << std::endl;

        }else{
            std::cout << "open sucess sibal" << std::endl;
        }
        void *shmaddr;
        shmaddr = shmat(_shmid, (void *)0, 0);
        if((_addr) == (void *)-1){
            perror("Shmat failed");
            return;

        }
        _addr = (uint8_t*)shmaddr;
    }
    ~Communicate(){
        if(!_addr)
            return;
        if(shmdt(_addr) == -1)
        {
            perror("Shmdt failed");
            return;
        }
    }
    void getData(uint8_t *data,int size){
        SharedMemoryRead(data,size);
    }
    uint8_t* _addr = NULL;

private:
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

    int _shmid;

};