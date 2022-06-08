//SO IS1 222B LAB10
//Bartosz Sobański
//sb49402@zut.edu.pl
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
extern char **environ;
int main(int argc, char *argv[]){
	char cwd[256];
	char *pol;
	size_t r_pol=32;
	pol=(char *)malloc(r_pol*sizeof(char));

	while(1){
		getcwd(cwd, sizeof(cwd));
		printf("Current path:  %s\n",cwd);

		printf("Instruction: ");
		getline(&pol, &r_pol, stdin);//wypis
		pol[strcspn(pol, "\n")] =0;

		//Podzial na slowa
		char* order = strtok(pol, " ");//wydzielenie pierwszego slowa
		char* flags[6];
		flags[0]=order;
		flags[1]=strtok(NULL, " ");
		char* order_1 = flags[1];
		flags[2]=strtok(NULL, " ");
		char* order_2 = flags[2];
		flags[3]=strtok(NULL, " ");
		char* order_e = flags[3];
		char* order_l;//ostatni
		//Iteracja po reszcie
		for (int i=4;i<=6;i++){
			flags[i] = strtok(NULL, " ");
		}
		//Wydzielenie ostatniego
		for (int i=0;i<=6;i++){
			if(flags[i] == NULL){
				order_l = flags[i-1];
				break;
			}
		}
		//Wydzielenie ostatniej litery
		int last_l=strlen(order_l);//dlugosc ostatniego
		char* order_c = &order_l[last_l-1];//ostatnia litera
		//IFY
		if((strcmp(order, "exit")==0)){
			printf("Good bye\n");
			return 0;
		}// EXIT
		else if(strcmp(order_c, "&")==0){
			for (int i=0; i<=6; i++){
				if(flags[i] == NULL){
					order_l[strlen(order_l)-1] = '\0';
					flags[i-1] = order_l;
					break;
				}
			}
			pid_t comm;
			comm = fork();
			if(comm == 0){
				execvp(flags[0],flags);
				printf("There is no such program\n");
				return 2137;
			}
		}// &
		else{
		if((strcmp(order, "vars")==0)){
			int i=0;
			while(environ[i] != NULL){
				printf("%s\n",environ[i]);
				i++;
			}
		}// VARS
		else if((strcmp(order,"set\0")==0)){
			if((order_1 != NULL)&&(order_2 != NULL)){
				if(order_e != NULL){
					printf("Too many arguments\n");
				}else{
					order_2[strcspn(order_2,"\n")]=0;
					setenv(order_1,order_2,1);
				}
			}else{
				printf("Too few arguments\n");
			}
		}// SETENV
		else if((strcmp(order,"del\0")==0)){
			if(order_1 != NULL){
				if(order_2 != NULL){
					printf("Too many arguments\n");
				}else{
					order_1[strcspn(order_1, "\n")]=0;
					unsetenv(order_1);
				}
			}else if((order_1 == "\0")||(order_1 == NULL)){
				printf("Too few arguments\n");
			}
		}// UNSETENV
		else if((strcmp(order,"cd\0")==0)){
			if(order_1 != NULL){
				if(order_e != NULL){
					printf("Too many arguments\n");
				}else{
					order_1[strcspn(order_1, "\n")]=0;
					chdir(order_1);
				}
			}else{
				printf("Too few arguments\n");
			}
		}// CD
		else{
			pid_t comm;
			comm = fork();
			if(comm == 0){
				execvp(flags[0],flags);
				printf("There is no such program\n");
				return 2137;
			}else{
				int exit = 1;
				if(waitpid(comm,&exit,0)>0 && WEXITSTATUS(exit)!=2137){
					 printf("Exit code: %d \n", WEXITSTATUS(exit));
				}
			}
		}//File_&_error
		}
	}//end of while/program
	return 0;
}

