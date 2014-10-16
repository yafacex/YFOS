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

bool isHex(string number){
    string pre = number.substr(0,2);
    return pre == "0x";
}

unsigned long addr2long(string hex){
    bool bHex = false;
    if (isHex(hex)) {
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
    if (!bHex) {
        //Oct
        unsigned long number = 0;
        for (int i = 0; i < hex.size(); ++i) {
            number = number * 10 + hex.at(i) - '0';
        }
        return number;
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
    result[1] = int2hex(c & 0x000f);
    result[0] = int2hex(c>>4 & 0x000f);
    return string(result,result+2);
}

void copySeg(vector<string>& args){
    if (args.size() != 7) {
        printf("Copy Seg Usage:\n -cp [src filename] [src from] [src to] [dest filename] [dest from]\n\n * means end of file\n");
        return;
    }
    
    string srcFile = args[2];
    string srcFrom = args[3];
    string srcTo = args[4];
    string destFile = args[5];
    string destFrom = args[6];
    
    FILE* from = fopen(srcFile.c_str(), "r");
    FILE* to = fopen(destFile.c_str(), "at+");
    
    long srcFromAddr = addr2long(srcFrom);
    long srcToAddr = 0;
    if (srcTo == "*") {
        fseek(from, 0, SEEK_END);
        srcToAddr = ftell(from);
    }else{
        srcToAddr = addr2long(srcTo);
    }
    long destFromAddr = addr2long(destFrom);
    
    long length = srcToAddr - srcFromAddr;
    
    fseek(from, srcFromAddr, SEEK_SET);
    fseek(to, destFromAddr, SEEK_SET);

    void* buffer = (void*)malloc(length);
    
    fread(buffer, 1, length, from);
    fwrite(buffer, 1, length, to);
    
    printf("from:%s\n",srcFile.c_str());
    printf("to:%s\n",destFile.c_str());
    printf("write %ld bytes from %s to %s\n",length,srcFrom.c_str(),destFrom.c_str());
}

void log(vector<string>& args){
    if (args.size() != 5) {
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
            }else if (i%2 == 0) {
                printf("\t0x");
            }
            c = fgetc(fp);
            printf("%s",char2hex(c).c_str());
        }
        printf("\n\n");
    }
}

void trim(vector<string>& args){
    if (args.size() != 5) {
        printf("Log Seg Usage:\n -log [src filename] [src from] [src to]\n\n");
    }
    string srcFile = args[2];
    string srcFrom = args[3];
    string srcTo = args[4];
    long fromAddr = addr2long(srcFrom);
    long toAddr = addr2long(srcTo);
    long length = toAddr - fromAddr;
    
    FILE* fp = fopen(srcFile.c_str(), "wb+");
    if (fp) {
        void* buffer = malloc(length);
        fseek(fp, fromAddr, SEEK_SET);
        fread(buffer, 1, length, fp);
        fseek(fp, 0, SEEK_SET);
        fwrite(buffer, 1, length, fp);
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
        args.push_back("-cp");
        args.push_back("./iplo2.img");
        args.push_back("0");
        args.push_back("512");
        args.push_back("./iplo.img");
        args.push_back("512");
    }
//    {
//        args.push_back("-log");
//        args.push_back("./iplo.img");
//    }
    
    if (args.size() == 1) {
        printf("Pls Input Operate!\n -cp / -log\n");
        return 1;
    }
    
    printf("\n\n*************************\n\n");
    
    string op = args[1];
    if (op == "-cp") {
        copySeg(args);
    }else if(op == "-log"){
        log(args);
    }else if(op == "-trim"){
        trim(args);
    }else{
        printf("Pls Input Operate!\n -cp / -log\n");
    }
    return 1;
}

