/**
 * @file qlinkedlistat.cpp
 * @brief Helper for QLinkedList
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "qlinkedlistat.h"

#ifndef QLINKEDLISTAT_CPP
#define QLINKEDLISTAT_CPP

template<class T>
T QLinkedListAt(QLinkedList<T> &list, int N)
{
    Q_ASSERT(!list.isEmpty());
    typename QLinkedList<T>::iterator it = list.begin();
    for(int i = 0; i < N; i++)
    {
        it++;
        if(it == list.end())
        {
            it--;
            break;
        }
    }
    return *it;
}

#endif // QLINKEDLISTAT_CPP
