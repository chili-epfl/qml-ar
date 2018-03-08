#ifndef QLINKEDLISTAT_H
#define QLINKEDLISTAT_H

#include <QLinkedList>
#include <QImage>

// get N'th element in qlinkedlist (starting from 0)
template<class T> T QLinkedListAt(QLinkedList<T> &list, int N);

#include "qlinkedlistat.cpp"

#endif // QLINKEDLISTAT_H
