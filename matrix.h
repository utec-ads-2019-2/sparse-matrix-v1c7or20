#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include "node.h"

using namespace std;


template <typename T>
class Matrix {
private:
    Node<T> *root;
    unsigned int rows, columns;

public:
    Matrix(unsigned  rows, unsigned  columns){
        root = new Node<T>;
        root->posX=-1;
        root->posY=-1;
        root->data=INT_MAX;
        root->down = new Node<T>;
        root->down->posX=-1;
        root->down->posY=0;
        root->next = new Node<T>;
        root->next->posY=-1;
        root->next->posX=0;
        Node<T>* actual = root->down;
        for (int i = 1; i < rows ; ++i) {
            actual->down = new Node<T>;
            actual=actual->down;
            actual->posY=i;
            actual->posX=-1;
        }
        actual = root->next;
        for (int j = 1; j < columns; ++j) {
            actual->next = new Node<T>;
            actual= actual->next;
            actual->posY=-1;
            actual->posX=j;
        }
        this->rows=rows; this->columns=columns;
    }

    void set(unsigned  row, unsigned  column, T data){
        if(row<=rows and column<=columns){
            Node<T>* rowNode=root->down;
            Node<T>* columnNode=root->next;
            Node<T>* toInsert = new Node<T>;
            toInsert->posX=row;
            toInsert->posY=column;
            toInsert->data = data;
            for (int i = 0; i <row ; ++i) {
                rowNode= rowNode->down;
            }
            for (int i = 0; i <column ; ++i) {
                columnNode= columnNode->next;
            }
            Node<T>* before=rowNode;
            Node<T>* after= before->next;
            while (before->next != nullptr and after != nullptr  and after->posX<column ){
                if(after->posX<column)
                before = before->next;
                after = before->next;
            }
            before->next=toInsert;
            toInsert->next = after;
            before=columnNode;
            after=before->down;
            while (before->down!= nullptr and after!= nullptr and after->posY<row ){
                before = before->down;
                after = before->down;
            }
            before->down=toInsert;
            toInsert->down= after;
        } else
            throw out_of_range("Row or column not in matrix");

    }
    T operator()(unsigned row, unsigned column){
        if(row<rows and column < columns){
            Node<T>* rowNode = root;
            for (int i = 0; i <= row; ++i) {
                rowNode= rowNode->down;
            }
            while (rowNode != nullptr){
                rowNode=rowNode->next;
                if(rowNode->posY == column){
                    return rowNode->data;
                }
            }
            throw invalid_argument("Element does not exist");
        }else
            throw out_of_range("Row or column not in matrix");
    }
    Matrix<T> operator*(T scalar) const{
        Node<T>* rowActual = root;
        Matrix<T>* answer = new Matrix<T>(rows,columns);
        for (int i = 0; i < rows; ++i) {
            rowActual= rowActual->down;
            Node<T>* nodeActual = rowActual;
            while (nodeActual->next!= nullptr){
                nodeActual=nodeActual->next;
                T toInsert=nodeActual->data*scalar;
                answer->set(nodeActual->posX,nodeActual->posY,toInsert);
            }
        }
        return *answer;
    }
    Matrix<T> operator*(Matrix<T> other) const{

    }
    Matrix<T> operator+(Matrix<T> other) const{
        Matrix<T>* answer = new Matrix<T>(rows,columns);
        
    }
    Matrix<T> operator-(Matrix<T> other) const{

    }
    Matrix<T> transpose() const{

    }
    void print() const{

    }

    ~Matrix(){
        delete root;
        root= nullptr;
    }
};

#endif //SPARSE_MATRIX_MATRIX_H