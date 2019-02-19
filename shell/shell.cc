#include "shell.hh"
#define INPUT_END 0
#define OUTPUT_END 1

int main()
{ 
    std::string promptline;
    std::ifstream inFile;
    inFile.open("prompt.txt");
    inFile >> promptline;
    inFile.close();

    std::string input;
    while(true)
    { 
        std::cout <<promptline<< ' ';
        std::getline(std::cin, input);
        if (input == "new_file") new_file();
        if (input == "ls") list();
        if (input == "find") find();
        if (input == "python") python();
        if (input == "quit") return 0;
        if (input == "error") return 1;
        //else {std::cout<<input<<std::endl;
        //    return 1;
        //} 
    } 
}

void new_file()
{ 
    std::string fileName;
    std::string inputstr = "teststring";
   
    std::cout<< "Geef bestandsnaam: ";
    std::getline(std::cin,fileName);
    fileName +=".txt";
    
    creat(fileName.c_str(),S_IRWXU);
    int fd = open(fileName.c_str(),O_WRONLY);
        
    while(true){
       std::cout<<">";
       std::getline(std::cin,inputstr);
       if(inputstr=="EOF"){
           close(fd);
           break;
       }
       inputstr+="\n";
       write(fd,inputstr.c_str(),inputstr.size());
    }
    
}

void list(){ 
    int status;
    char *args[2];

    args[0] = "/bin/ls";
    args[1] = NULL;
    
    if ( fork() == 0)
    execv("/bin/ls",args);
    else
    {wait( &status );}
}

void find(){
    __pid_t pid1;
    __pid_t pid2;
    int fd[2];

    char *zoekterm = new char(8);
    std::cout<< "What are you looking for?: ";
    std::cin >> zoekterm;
    
    char *args[2] = {zoekterm,NULL};
    char *argf[2] = {"./",NULL};
    pipe(fd);
    pid1=fork();
    if(pid1==0){//child 1
        close(fd[OUTPUT_END]);                    
        dup2(fd[INPUT_END], STDIN_FILENO);      
        close(fd[OUTPUT_END]); 
        execv("/usr/bin/find",argf);
    }
    else {
        pid2=fork();
        if(pid2==0){//child 2

            close(fd[INPUT_END]);               
            dup2(fd[OUTPUT_END], STDOUT_FILENO);  
            close(fd[INPUT_END]);
            execv("/usr/bin/grep",args);
        }else{//parent

        int status1;
        int status2;
        close(fd[INPUT_END]);                   
        close(fd[OUTPUT_END]);                   
        waitpid(pid2, &status2, 0); 
        waitpid(pid1, &status1, 0); 
        
        } 
    }

}

void python()
{ system("/usr/bin/python");}