/**
 * @file qlinkedlistat.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef QLINKEDLISTAT_H
#define QLINKEDLISTAT_H

#include <QLinkedList>
#include <QImage>

    /**
    * @brief Get N'th element in qlinkedlist (starting from 0)
    */
template<class T> T QLinkedListAt(QLinkedList<T> &list, int N);

#include "qlinkedlistat.cpp"

#endif // QLINKEDLISTAT_H
