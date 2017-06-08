#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int exec(char* cmd, char* args[]);
char* readFile(char* file);
int parseConfig(char* file);
char* runProgram(char* fileLocation);
void writeToCsv(char* name, char* score);
void processPrograms(char* dirPath, char* input, char* output);
int compile(char* file);
char* getFileLocation(char* file);
char* concatStrings(char* c1, char* c2,char* separator);

FILE *f;
char* dir;
char* input;
char* output;

/****************************************** 
* Shir Zrachya & Yonatan Berg 
* 308417823 & 311123327 
* 661238
* Exercise 2 
******************************************/ 
int main(int argc, char** argv){
	f = fopen("results.csv","a");
	if(f == NULL)
		return -1;
	printf(f,"Running...\n");
	if(parseConfig(argv[1]) == 1){
		printf("Could not read config file.\nExiting.");
		exit(-1);
	}
	processPrograms(dir,input,output);
	printf(f,"Finished processing.\nGood luck :).");
	return 0;
}

/**************************************************************
 * Parsing the program's variables from the configuration file.
 **************************************************************/
int parseConfig(char* file){
	FILE* conf; 
	size_t dirlen = 0;
	size_t inputlen = 0;
	size_t outputlen = 0;
	char* inputFile;
	char* outputFile;
	
	// Opening the config file.
	conf = fopen(file,"r");
	if(conf == NULL){
		// Couldn't open the config file.
		return 1;
	}
	
	// Reading the root directory path.
	getline(&dir,&dirlen,conf);
	strtok(dir,"\n");
	// Reading the programs input.
	getline(&inputFile,&inputlen,conf);
	strtok(inputFile,"\n");
	input = readFile(inputFile);
	// Reading the wanted output.
	getline(&outputFile,&outputlen,conf);
	strtok(outputFile,"\n");
	output = readFile(outputFile);

	return 0;
}

/***********************************************************************
 * Reads the content of a given file.
 **********************************************************************/
char* readFile(char* file){
	FILE* f;
	size_t len = 0;
	ssize_t read;
	char* buf = NULL;
	char* buf2 = "";
	f = fopen(file,"r");
	// Reading till the end of the file.
	while((read = getline(&buf,&len,f)) != -1){
		buf2 = concatStrings(buf2,buf,"");
	}
	return buf2;
}

/****************************************************************************************************************
 * Going through every program in the given root directory, recursively, compiling it if possible, and running it
 * to see if it gives the wanted output. Writing to the output csv file according to the results.
 ****************************************************************************************************************/
void processPrograms(char* dirPath,char* input, char* output){
	DIR* dir; // Directory
	struct dirent *entry; // entity
	
	// Checking if the directory exists.
	if (!(dir = opendir(dirPath))){ 
		return;
	}
	
	// Running on sub-directories.
	while ((entry = readdir(dir)) != NULL){
		printf("%s\n",entry->d_name);
		
		// Checking if the current entry is a directory.
		if(entry ->d_type == DT_DIR){
			// Avoiding .'s and ..'s 
		    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			// Processing the current sub-directory.
			char* path = concatStrings(dirPath,entry->d_name,"/");
		        processPrograms(path,input,output);
		}
		// Compiling the current file if it's a .c file.
		else if(entry->d_type == DT_REG && strstr(entry->d_name,".c") != NULL){
			int ret = 0;
			char* file = concatStrings(dirPath,entry->d_name,"/");
			char* fileLocation = getFileLocation(file);
			// Compiling the current program.
			ret = compile(file);
			
			// Checking if the program was compiled successfuly.
			if(ret != 0){
				writeToCsv(fileLocation,"0");
			}
			else{
				// Running the program
				char* res = runProgram(fileLocation);
				// Comparing the programs output and the wanted output.
				if(strcmp(output,res) == 0){
					writeToCsv(fileLocation,"100");
				}
				else{
					writeToCsv(fileLocation,"0");
				}
			}
		}
	} 
	closedir(dir);
}

/*************************
 * Runs the given program.
 *************************/
char* runProgram(char* fileLocation){
	// Don't want to override the pointers
	char* file = strdup(fileLocation);
	char* args = strdup(input);
	// Command is "file args", i.e file="/bin/ls", args="-l" =>> cmd="/bin/ls -l"
	char* cmd = concatStrings(file,args," ");
	// Run the file
	FILE *prog = popen(cmd, "r");
	char buf[2048];
	// Get the output, place it in the buffer
	fgets(buf, sizeof(buf), prog);
	// Close the execution
	pclose(prog);
	// Return new representation of output
	return strdup(buf);
}

/*********************************************
 * Writes a line to the results file.
 ********************************************/
void writeToCsv(char* name, char* score){
	fprintf(f,concatStrings(name,score,","));
	fprintf(f,"\n");
}

/***************************************************************************************************
 * Compiles the given file.
 ***************************************************************************************************/
int compile(char* file){
	int i =0;
	char *cmd = "/usr/bin/gcc";
	char *args[] = {strdup(cmd),"-s","-o",strdup(getFileLocation(file)),file,NULL};
	i = exec(cmd,args); 
	return i;
}

/**************************************************************************************
 * Executes a shell command
 **************************************************************************************/
int exec(char* cmd, char* args[]){
	int pid;
	int res = 0;
	pid = fork();
	if(pid < 0)
		return -1; 
	else if(pid == 0){ 
		res = execv(cmd,args);
	}	
	else{ 
		wait(pid);
	}
	return res;
}

/*******************************************************************************
 * Get the compilation output files path.
 ******************************************************************************/
char* getFileLocation(char* f){
	char* loc;
	char* name = "out.o";
	char* file = strdup(f);
	char* dir = dirname(file);
	loc = concatStrings(dir,name,"/");
	return loc;
}

/*******************************************************************
 * Concatenates two strings with a seperator between them.
 ******************************************************************/
char* concatStrings(char* c1, char* c2,char* separator){
	char* new;
	char* a = strdup(c1);
	char* b = strdup(c2);
	new = malloc(strlen(a)+strlen(separator)+strlen(b)+1);
	strncpy(new,a,strlen(a));
	strncat(new,separator,strlen(separator));
	strncat(new,b,strlen(b));
	strncat(new,"\0",1);
	return new;
}
