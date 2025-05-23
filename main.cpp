#include <iostream>
#include "SkipList.h"

int main() {
    SkipList list;
    char key;
    int choice;
    list.setQuit(false);
    list.display();
    list.update();
    printf("Would you like to:\n\t1) Control Flow with keyboard to watch animations?"
           "\n\t2) Automatically insert 2,000 elements with partial implementation shown?\n");
    cin >> choice;

    switch (choice) {
        case 1:
            while (!list.getQuit()) {
                if (list.kbhit()) {
                    key = list.getKey();
                    switch (key) {
                        case 'r':
                            list.insert(rand() % 10000);
                            break;
                        case 'q':
                            list.setQuit(true);
                            break;
                    }
                    list.display();
                    list.update();
                }
            }
            break;
        case 2:
            while (!list.getQuit()) {
                if (list.kbhit()) {
                    key = list.getKey();
                    for (int i = 0; i < 2000; i++) {
                        list.insert(rand() % 10000, false);
                        list.update();
                    }
                    list.display();
                    list.update();
                }
            }
    }
    return 0;
}
