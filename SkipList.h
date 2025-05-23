//
// Created by Carter Lewis on 4/19/25.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL_Plotter.h"
#include <thread>
#include <chrono>

struct node {
public:
    int val; //value of node
    node **next; //array of pointers to next nodes
    int size;

    node(int val, size_t size) : val(val), next(new node *[size + 1]), size(size) {}
};

const int SDL_WIDTH = 1500;
const int SDL_HEIGHT = 1000;

class SkipList : public SDL_Plotter {
private:
    const int MAX_HEIGHT = 11; // Log2(2000) - approximately equal distribution
    int currentLevel; //Height of tallest "tower"
    node *head; // "Top right" node of skip list
    int size;

    point findCoordinates(int val) {
        node *curr = head;
        int i = SDL_WIDTH / (size + 2);
        int x = i;
        while (curr->val != val) {
            curr = curr->next[0];
            x += i;
        }
        return {x, 950 - (curr->size - 1) * 60};
    }

public:
    SkipList() : currentLevel(0), head(new node(-1, MAX_HEIGHT)), size(0), SDL_Plotter(SDL_HEIGHT, SDL_WIDTH) {
        srand(time(NULL));
    }

    node *newNode(int val, int height) {
        return new node(val, height);
    }

    void insert(int val, bool slow = true) {
        node *curr = head;
        node *list[MAX_HEIGHT + 1];

        for (int i = currentLevel; i >= 0; i--) {
            while (curr->next[i] != nullptr && curr->next[i]->val < val) {
                point p = findCoordinates(curr->val);
//                display();
                displayNode(curr->val, p.x, p.y, {255, 0, 0});
                update();
                if (slow) this_thread::sleep_for(chrono::seconds(2));
                curr = curr->next[i];
            }
            list[i] = curr;
        }

        point p = findCoordinates(curr->val);
        displayNode(curr->val, p.x, p.y, {0, 255, 0});
        update();
        if (slow) this_thread::sleep_for(chrono::seconds(2));
        curr = curr->next[0];
        size++;

        if (curr == nullptr || curr->val != val) {
            int l = 1;
            while (l < this->MAX_HEIGHT && rand() % 2 == 1) {
                l++;
            }

            clear();


            if (l > currentLevel) {
                for (int i = currentLevel + 1; i < l; i++) {
                    list[i] = head;
                }
                currentLevel = l;
            }
            node *insertNode = newNode(val, l);

            for (int i = 0; i < l; i++) {
                insertNode->next[i] = list[i]->next[i];
                list[i]->next[i] = insertNode;
            }
            animateTowerBuilding(val, l, slow);
        }
        clear();
        display();
        update();
    }

    void display() {
        int x_jump = SDL_WIDTH / (size + 2);
        int x = x_jump;
        node *curr = head;
        for (int i = 0; i < 11; i++) {
            for (int j = SDL_WIDTH / (size + 2); j < SDL_WIDTH - (SDL_WIDTH / (size + 2)); j++) {
                plotPixel(j, 950 - i * 60, {0, 0, 0});
            }
        }
        while (curr) {
            for (int i = 0; i < curr->size; i++) {
                displayNode(curr->val, x, 950 - i * 60);
            }
//            displayNode(curr->val, x, 450);
            curr = curr->next[0];
            x += x_jump;
        }
    }

    void animateTowerBuilding(int val, int l, bool slow = true) {
        int x_jump = SDL_WIDTH / (size + 2);
        int x = x_jump;
        int xVal;
        node *curr = head;
        while (curr) {
            if (curr->val != val) {
                for (int i = 0; i < curr->size; i++) {
                    displayNode(curr->val, x, 950 - i * 60);
                }
            } else {
                xVal = x;
            }
            curr = curr->next[0];
            x += x_jump;
        }
        for (int i = 0; i < l; i++) {
            displayNode(val, xVal, 950 - i * 60);
            update();
            if (slow) this_thread::sleep_for(chrono::seconds(1));
        }
    }
};

#endif //SKIPLIST_SKIPLIST_H
