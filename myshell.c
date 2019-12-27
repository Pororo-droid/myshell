#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define MAX_LINE_LENGTH 127
#define MAX_LINES 10
#define DEBUG //printf("1");

char* cwd; //현재 디렉토리의 위치

char* read_line(){
    int bufsize = MAX_LINE_LENGTH;
    char* buf = (char*)malloc(sizeof(char)*bufsize);
    int position = 0;
    int c;

    if(!buf){
        printf("Shell: allocation error");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();

        if(c == EOF || c == '\n' || c == 10){
            buf[position] = '\0'; //NULL
            return buf;
        }
        else{
            buf[position++] = c;
        }

        if(position >= bufsize){
            bufsize += MAX_LINE_LENGTH;
            buf = realloc(buf,bufsize);
            if(!buf){
                printf("Shell: allocation error");
                exit(EXIT_FAILURE);
            }
        }
        
    }

}


int shell_execute(char** lines){
    int i = 0;
    if(lines[i] == NULL)
        return 1;

    if(strcmp(lines[i],"exit") == 0){
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(lines[i],"ls") == 0){ 
        DIR* dir = NULL;
        struct dirent* entry = NULL;
        i++;

        if((dir = opendir(cwd)) == NULL){
            printf("current directory error \n");
            return 1;
        }

        while((entry = readdir(dir)) != NULL){
            if(entry->d_name[0] != '.')
                printf("%s\n",entry->d_name);
        }
        
        while(lines[i]){
            if(strcmp(lines[i],"-a") == 0){
                dir = opendir(cwd);
                while((entry = readdir(dir)) != NULL){
                    if(entry->d_name[0] == '.')
                        printf("%s\n",entry->d_name);
                }
            }
            i++;
        }
        
    }
    else if(strcmp(lines[i],"cd") == 0){
        char* ptr;
        DIR* dir = NULL;
        struct dirent* entry = NULL;
        i++;

        char* line = lines[i];
        
        ptr = strtok(line,"/");

        while(ptr != NULL){
            if(chdir(ptr) != 0){
                printf("There is no such directory");
                return 1;
            }
            ptr = strtok(NULL,"/");
        }
    }
    else if(strcmp(lines[i],"pwd") == 0){
        printf("%s\n",cwd);
    }

    return 1;
}

char** split_line(char* line){
    int i = 0 , bufsize = MAX_LINES;
    
    /*if(strcmp(line,"") == 0){
        return NULL;
    }
    */
    char** lines = malloc(sizeof(char*)* bufsize);
    

    char* ptr = strtok(line," ");

    while(ptr != NULL){
        lines[i++] = ptr;
        ptr = strtok(NULL," ");

        if(i >= bufsize){
            bufsize += MAX_LINES;
            lines = realloc(lines,sizeof(char*) * bufsize);
            if(!lines){
                printf("Shell: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
    return lines;
}

void myshell_loop(void){
    char* line;
    char** lines;
    int status;

    do{
        cwd = (char*)malloc(sizeof(char)*1024);
        getcwd(cwd,1024);

        printf("> ");
        line = read_line();
        lines = split_line(line);

        //status = 1;
        status = shell_execute(lines);
    }while(status);

} 

int main(void){
    printf("%c[1;32m",27);
    myshell_loop();

    return 0;
}