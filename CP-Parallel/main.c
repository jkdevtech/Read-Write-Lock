#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NTHREADS 100

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define SIZE 1024



int offset = 0;
int base = 0, read_fd, write_fd, n_thread;
void *read_write(void *);


void *read_write(void *ptr)
{
	int seek = 1024, block_size = 0;
	char buffer[SIZE];
	int thread_num = (int) ptr;
	printf("thread number == %d\n", thread_num);
        lseek(read_fd, seek*thread_num, SEEK_SET);
        while ((block_size = read(read_fd, buffer, SIZE)) > 0) {
                lseek(write_fd, seek*thread_num, SEEK_SET);
                if (write(write_fd, buffer, SIZE) == -1)
                        printf("Error while writing");
		thread_num += 3;
        	lseek(read_fd, seek*thread_num, SEEK_SET);
        }
	return 0;
}

int main(int argc, char **argv)
{
	pthread_t thread_id[NTHREADS];
	int i, j, num;


	if (argc != 3) {
                printf("Can't perform write operation\n");
                exit(1);
        }


        read_fd = open(argv[1], O_RDONLY, 644);
        if (read_fd == -1) {
                printf("Can not open read file\n");
                exit(2);
        }

        if ((write_fd = creat(argv[2], MODE)) == -1) {
                printf("can not create %s\n", argv[2]);
                exit(3);
        }


	scanf("%d", &num);
	for(i=0; i < num; i++) {
		pthread_create(&thread_id[i], NULL, read_write, (void *) i);
	}

	for(j=0; j < num; j++) {
		pthread_join( thread_id[j], NULL); 
	}
	return 0;
}
