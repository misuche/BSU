#pragma once
#ifndef RECEIVER_MESSAGE_H
#define RECEIVER_MESSAGE_H

#include<iostream>
#include<fstream>
#include<Windows.h>


struct Message {
    Message() {}

    Message(std::string str) {
        strcpy(message, str.c_str());
    }

    char *toString() {
        return message;
    }

    friend std::fstream &operator<<(std::fstream &file, const Message &msg) {
        file.write((const char *) &msg, sizeof(struct Message));
        return file;
    }

    friend std::fstream &operator>>(std::fstream &file, Message &msg) {
        file.read((char *) &msg, sizeof(struct Message));
        return file;
    }

private:
    char message[20];
};

#endif //RECEIVER_MESSAGE_H