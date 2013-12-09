#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <mpi.h>

FILE *popen(const char *command, const char *type); 

int nv_get_temperature (int *temperature)
{
    int err = EIO;
	char token[32];
    FILE *fp = popen("nvidia-smi -q", "r");

    if (fp == NULL) {
        err = errno;
        perror("nvidia-smi");
        return err;
    }
       
     
    
    while (1 == fscanf(fp, " %31s ", token)) {
        if (0 == strcmp(token, "Temperature")) {			
			fscanf(fp, "%s", token);
			if (0 == strcmp(token, "Gpu")) {			
				if (1 == fscanf(fp, " : %4s C", token)) {
					*temperature = atoi(token);
					err = 0;  // success 
					break;
				}
			}
        }
    }
    fclose(fp);
    if (err)
        fprintf(stderr, "nvidia-smi did not report the temperature\n");
    return err;
}

int nv_get_memory (int *total_memory, int *used_memory)
{
    int err = EIO;
	char token[32];
    FILE *fp = popen("nvidia-smi -q", "r");

    if (fp == NULL) {
        err = errno;
        perror("nvidia-smi");
        return err;
    }
       
     
    
    while (1 == fscanf(fp, " %31s ", token)) {
		
        if (0 == strcmp(token, "Memory")) {
			
			fscanf(fp, "%s", token);
			if (0 == strcmp(token, "Usage")) {	
					
				fscanf(fp, "%s", token);
				
				if (0 == strcmp(token, "Total")) {			
					if (1 == fscanf(fp, " : %4s MB", token)) {
						*total_memory = atoi(token);
					}
				}
				fscanf(fp, "%s", token);
				if (0 == strcmp(token, "Used")) {			
					if (1 == fscanf(fp, " : %4s C", token)) {
						*used_memory = atoi(token);
						err = 0;  // success 
						break;
					}
				}
			}
		}
	}
    fclose(fp);
    if (err)
        fprintf(stderr, "nvidia-smi did not report the memory\n");
    return err;
}
/*                                         
int main (int argc, char **argv)                                         
{       
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	                                            
    int err, temperature,total_memory,used_memory;                                   
                                                
    err = nv_get_temperature(&temperature);                         
    if (!err)                                           
        printf("Temperature: %d C\n", temperature);  
    err = nv_get_memory(&total_memory,&used_memory);
    if (!err)                                           
        printf("Total Memory: %d MB\nUsed memory: %d MB\n", total_memory,used_memory);                     
    return err;                                         
}
* */
