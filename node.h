#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
    T data;
    int posX,posY;

public:
    explicit Node(){
        next = nullptr;
        down = nullptr;
    }

    void deleteRow(){
        if (this->next!= nullptr){
            this->next->deleteRow();
            this->next= nullptr;
        }
        delete this;
    }

    void deleteColumn(){
        if (this->down!= nullptr){
            this->down->deleteColumn();
            this->down = nullptr;
        }
        this->deleteRow();
     }

    ~Node(){
    }

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
