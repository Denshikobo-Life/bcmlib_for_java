// bcm_interface.c

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "bcm_interface.h"

//extern char **environ;

int child;
int fd;
int local_sync_code;

int bi_status;

void do_child( char *args )
{
    char *new_argv[3];
    new_argv[0] = "../bcm2835_for_java/bcm2835_for_java";
    new_argv[1] = args;
    new_argv[2] = NULL;
    execv(new_argv[0], new_argv );
    printf("execv failed %s %s\n",new_argv[0],new_argv[1]);
    return;
}

int bi_init( char *args )
{
char *s;
//int len;
//int pid;
//int loop;

    if( args == NULL )
    {
        printf("bcm_interface needs 1 parameter like as /tmp/shared_mem\n");
      exit ( -1 );
    }
    else
    {
      fd = open( args , O_RDWR);
      if( fd == -1 )
      {
        printf("file %s can't open\n",args);
        exit(-1);
      }
      else
      {
         s = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
         if( s == MAP_FAILED )
         {
            printf("we can't create mapping file\n");
         }
      }
    }

    w_buff = (struct ring_buff *)s;
    r_buff = (struct ring_buff *)(s+sizeof(struct ring_buff) );
    bi_send_buff = (char *)( s + 2*sizeof(struct ring_buff) );
    bi_rec_buff = (char *)( bi_send_buff + TEMP_BUFF_SIZE );
    init_ring_buff(w_buff,WRITER_INIT);
    init_ring_buff(r_buff,READER_INIT);

    sync_code = (int *)( bi_rec_buff + TEMP_BUFF_SIZE );
    reply_code = (int *)( sync_code + 1 );
    local_sync_code = 0;
    
    child = fork();
    if( child < 0 )
    {
       printf("we can't create child\n");
    }
    else if( child == 0 )
    {
        do_child(args);
        exit(0);
    }

    printf("bcm_interface ver1.00 start \n");
    if( context_switch(100) != 0 )
    {
        printf("bi_status = %d  @bi_init \n",bi_status );
        return -1;
    }
    else if( ope_hello("Hello child!") != 0 )
    {
        printf("Ring buffer failed bi_status=%d\n",bi_status );
        return -1;
    }

    return ope_init();    // bcm2835_init()
}

int bi_close(void)
{
    ope_close();     // bcm2835_close())
    if( ope_exit() != 0 )
    {
        printf("bi_status = %d  @bi_close \n",bi_status );
        return -1;
    }
    close( fd );
    kill(child,SIGINT);
    printf("Close bcm_interface\n");
    return 0;
}

//#define PIN RPI_GPIO_P1_11

