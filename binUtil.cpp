//
//  main.cpp
//  BinUtil
//
//  Created by yafacex on 14-10-15.
//  Copyright (c) 2014年 yafacex. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isAlphet(char c){
    if (c >= 'a' && c <= 'z') {
        return true;
    }
    if (c >= 'A' && c <= 'Z') {
        return true;
    }
    return false;
}

char toLowerCase(const char c){
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return 'a';
}

unsigned long addr2long(string hex){
    bool bHex = false;
    if (hex.size() > 2) {
        string pre = hex.substr(0,2);
        if (pre == "0x") {
            bHex = true;
            unsigned long number = 0;
            for (int i = 2; i < hex.size(); ++i) {
                if (isAlphet(hex.at(i))) {
                    char lower = toLowerCase(hex.at(i));
                    int val = lower - 'a';
                    number = (number << 4) + val;
                }else{
                    int val = hex.at(i) - '0';
                    number = (number << 4) + val;
                }
            }
            return number;
        }
    }
    if (!bHex) {
        //Oct
        unsigned long number = 0;
        for (int i = 0; i < hex.size(); ++i) {
            number = number * 10 + hex.at(i) - '0';
        }
    }
    printf("Err:Bad Number [ %s ]\n",hex.c_str());
    return 0;
}

char int2hex(int a){
    if (a >= 0) {
        if (a < 10) {
            return a + '0';
        }else if(a < 16){
            return a - 10 + 'a';
        }else{
            printf("Bad int2hex transform [%d]",a);
            return 0;
        }
    }else{
        printf("Bad int2hex transform [%d]",a);
        return 0;
    }
}

string char2hex(char c){
    char result[3];
    result[0] = int2hex(c>>4 & 0x000f);
    result[1] = int2hex(c & 0x000f);
    return string(result,result+1);
}

void copySeg(vector<string>& args){
    if (args.size() < 2) {
        printf("Copy Seg Usage:\n -cp [src filename] [src from] [src to] [dest filename] [dest from] [dest to]\n\n");
    }
    string srcFile = args[2];
    string srcFrom = args[3];
    string srcTo = args[4];
    string destFile = args[5];
    string destFrom = args[6];
    string destTo = args[7];
}

void log(vector<string>& args){
    if (args.size() < 2) {
        printf("Log Seg Usage:\n -log [src filename] [src from] [src to]\n\n");
    }
    string srcFile = args[2];
    string srcFrom = args[3];
    string srcTo = args[4];
    FILE *fp = fopen(srcFile.c_str(), "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char c = 0;
        for (long i = 0; i < size; ++i) {
            if (i%512 == 0) {
                printf("\n[[[[sector %ld]]]]\n0x",i/512);
            }else if(i%16 == 0){
                printf("\n0x");
            }else if (i%4 == 0) {
                printf("\t0x");
            }
            c = fgetc(fp);
            printf("%s",char2hex(c).c_str());
        }
    }
}
int main(int argc, const char * argv[])
{
    vector<string> args;
    for (int i =0; i < argc; ++i) {
        args.push_back(string(argv[i]));
        printf("Arg : %s\n",args[i].c_str());
    }
    
    {
        args.push_back("-log");
        args.push_back("./BinUtil");
    }
    
    if (args.size() == 1) {
        printf("No Arg Input!\n");
        return 1;
    }
    
    printf("\n\n*************************\n\n");
    
    string op = args[1];
    if (op == "-cp") {
        copySeg(args);
    }else if(op == "-log"){
        log(args);
    }else{
        printf("Pls Input Operate!\n -cp / -log\n");
    }
    return 1;
}

