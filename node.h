#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
    T data;
    unsigned posX,posY;

public:
    explicit Node(){
        next = nullptr;
        down = nullptr;
    }
    ~Node(){
        if(next != NULL){
            delete next;
        }
        if (down != NULL){
            delete down;
        }
        next = nullptr;
        down = nullptr;
    }

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
