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
    int getRows(){ return rows;}
    int getColumns(){ return columns;}
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
            while (before->next != nullptr and after != nullptr  and after->posY<column ){
                before = before->next;
                after = before->next;
            }
            before->next=toInsert;
            toInsert->next = after;
            before=columnNode;
            after=before->down;
            while (before->down!= nullptr and after!= nullptr and after->posX<row ){
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
    const Matrix<T> operator*(T scalar) const{
        Node<T>* rowActual = root;
        Matrix<T> answer(rows,columns);
        for (int i = 0; i < rows; ++i) {
            rowActual= rowActual->down;
            Node<T>* nodeActual = rowActual;
            while (nodeActual->next!= nullptr){
                nodeActual=nodeActual->next;
                T toInsert=nodeActual->data*scalar;
                answer.set(nodeActual->posX,nodeActual->posY,toInsert);
            }
        }
        return answer;
    }
    const Matrix<T> operator*(Matrix<T>& other) const{
        Matrix<T> answer (rows,other.columns);
        if(other.rows == this->columns){
            Node<T>* actualThis=root->down;
            for (int i = 0; i < rows; ++i) {
                Node<T>* actualOther=other.root->next;
                for (int j = 0; j <other.columns ; ++j) {
                    Node<T>* actualIterThis=actualThis->next;
                    Node<T>* actualIterOther=actualOther->down;
                    T ansT = 0;
                    while (actualIterOther != nullptr and actualIterThis !=nullptr){
                        if (actualIterOther->posX > actualIterThis->posY){
                            actualIterThis = actualIterThis->next;
                        } else if (actualIterOther->posX < actualIterThis->posY){
                            actualIterOther = actualIterOther->down;
                        }else if (actualIterOther->posX == actualIterThis->posY) {
                            ansT += actualIterOther->data*actualIterThis->data;
                            actualIterOther = actualIterOther->down;
                            actualIterThis = actualIterThis->next;
                        }
                    }
                    answer.set(i,j,ansT);
                    actualOther = actualOther->next;
                }
                actualThis = actualThis->down;
            }
        }else throw invalid_argument("Matrices can not be multiplied");
        return answer;
    }
     const Matrix<T> operator+(Matrix<T>& other) const {
        Matrix<T> answer(rows,columns) ;
        if (this->columns == other.columns and this->rows == other.rows){
            Node<T>* nodeThis = root->down;
            Node<T>* nodeOther = other.root->down;
            for (int i = 0; i < rows ; ++i) {
                Node<T>* nodeThisIter = nodeThis->next;
                Node<T>* nodeOtherIter = nodeOther->next;
                while (nodeOtherIter != nullptr or nodeThisIter !=nullptr){
                    if (nodeOtherIter->posX > nodeThisIter->posX){
                        answer.set(nodeThisIter->posX,nodeThisIter->posY,nodeThisIter->data);
                        nodeThisIter = nodeThisIter->next;
                    } else if (nodeOtherIter->posX < nodeThisIter->posX){
                        answer.set(nodeOtherIter->posX,nodeOtherIter->posY,nodeOtherIter->data);
                        nodeOtherIter = nodeOtherIter->next;
                    }else if (nodeOtherIter->posX == nodeThisIter->posX) {
                        answer.set(nodeOtherIter->posX,nodeOtherIter->posY,nodeOtherIter->data+nodeThisIter->data);
                        nodeOtherIter = nodeOtherIter->next;
                        nodeThisIter = nodeThisIter->next;
                    }
                }
                nodeOther = nodeOther->down;
                nodeThis = nodeThis->down;
            }
            nodeOther = nodeThis = nullptr;
            return answer;
        }else
            throw invalid_argument("Matrix does not have equal rows or columns");
    }
     const Matrix<T> operator-(Matrix<T>& other) const {
         Matrix<T> answer(rows,columns);
        if(this->rows == other.rows and this->columns == other.columns){
            Node<T>* nodeThis = root->down;
            Node<T>* nodeOther = other.root->down;
            for (int i = 0; i < rows ; ++i) {
                Node<T>* nodeThisIter = nodeThis->next;
                Node<T>* nodeOtherIter = nodeOther->next;
                while (nodeOtherIter != nullptr or nodeThisIter !=nullptr){
                    if (nodeOtherIter->posX > nodeThisIter->posX){
                        answer.set(nodeThisIter->posX,nodeThisIter->posY,nodeThisIter->data);
                        nodeThisIter = nodeThisIter->next;
                    } else if (nodeOtherIter->posX < nodeThisIter->posX){
                        answer.set(nodeOtherIter->posX,nodeOtherIter->posY,(-1)*nodeOtherIter->data);
                        nodeOtherIter = nodeOtherIter->next;
                    }else if (nodeOtherIter->posX == nodeThisIter->posX) {
                        answer.set(nodeOtherIter->posX,nodeOtherIter->posY,(-1)*nodeOtherIter->data+nodeThisIter->data);
                        nodeOtherIter = nodeOtherIter->next;
                        nodeThisIter = nodeThisIter->next;
                    }
                }
                nodeOther = nodeOther->down;
                nodeThis = nodeThis->down;
            }
            nodeOther = nodeThis = nullptr;
            return answer;
        }else
            throw invalid_argument("Matrix does not have equal rows or columns");
    }

    void deleteNode(unsigned X, unsigned Y){
        Node<T>* before = root;
        Node<T>* toDelete = root->down;
        Node<T>* after = toDelete->next;
        for (int i = 0; i < X; ++i) {
            toDelete = toDelete->down;
        }
        before = toDelete;
        toDelete = toDelete->next;
        after = toDelete->next;
        for (int j = 0; j < Y; ++j) {
            if (toDelete->posX < X ){
                before = before->next;
                toDelete= before->next;
                after =toDelete->next;
            }else if (toDelete->posX == X and toDelete->posY == Y){
                j =X;
                break;
            } else throw out_of_range("element does not exist");
        }
        Node<T>* beforeY = root;
        Node<T>* toDeleteY = root->next;
        Node<T>* afterY = toDelete->next;
        for (int i = 0; i < Y; ++i) {
            toDeleteY = toDeleteY->next;
        }
        beforeY = toDeleteY;
        toDeleteY = toDeleteY->down;
        afterY = toDeleteY->down;
        for (int j = 0; j < X; ++j) {
            if (toDelete->posY < Y){
                beforeY = beforeY->down;
                toDeleteY= beforeY->down;
                afterY =afterY->down;
            }else if (toDelete->posY == Y){
                break;
            } else throw out_of_range("element does not exist");
        }
        before->next = after;
        beforeY->down = afterY;
        delete(toDelete);
        toDeleteY = 0;
    }

    const Matrix<T>& transpose() const{
        Matrix<T> answer(columns,rows);
        Node<T>* actual=root;
        for (int i = 0; i < rows; ++i) {
            actual= actual->down;
            Node<T>* nodeActual = actual->next;
            while (nodeActual!= nullptr){
                answer.set(nodeActual->posY,nodeActual->posX,nodeActual->data);
                nodeActual=nodeActual->next;
            }
        }
        return answer;
    }
    void print() const{
        Node<T>* actual=root;
        for (int i = 0; i < rows; ++i) {
            actual= actual->down;
            Node<T>* nodeActual = actual;
            while (nodeActual->next!= nullptr){
                for (int j = 0; j < (nodeActual->posX - nodeActual->posX); ++j) {
                    cout<<" ";
                }
                nodeActual=nodeActual->next;
                cout<<(nodeActual->data)<<" ";
            }cout<<endl;
        }
    }
    void operator=(Matrix<T> other){
        root->deleteColumn();
        root = nullptr;
        this->root = other.root;
        this->rows = other.rows;
        this->columns = other.columns;
        other.root = nullptr;
    }
    ~Matrix(){
        if(this->root != nullptr)
            this->deleteMatrix();
    }
    void deleteMatrix(){
        root->deleteColumn();
    }
};

#endif //SPARSE_MATRIX_MATRIX_H