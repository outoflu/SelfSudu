#include <iostream>
#include <string>
#include <cstring>
#include "./include/scene.h"
#include "./include/input.h"
#include "./src/command.cpp"
#include "./src/scene.cpp"

static void printHelp(){
    std::cout<<std::endl;
    std::cout<<" a little game in command line"<<std::endl;
    std::cout<<"Usage:"<<std::endl;;
    std::cout<<"\t Shufu [-l <progressFile>]"<<std::endl<<std::endl;
    std::cout<<"Options:"<<std::endl;
    std::cout<<"\t -l <path> \t specify path of progress file to load,optional."<<std::endl<<std::endl;
}

int main(int argc,char** argv){
    Scene scene;
    if (argc==1){
        int eraseGridNumber =inputDifficulty();
        //std::cout<<eraseGridNumber<<std::endl;
        scene.generate();
        scene.eraseRandomGrids(eraseGridNumber);
    }else if (argc==3&&!strcmp(argv[1],"-l")){
        scene.load(argv[2]);
    }else {
        printHelp();
    }
    scene.setMode(inputKeyMode());
    scene.play();
    return 0;
}