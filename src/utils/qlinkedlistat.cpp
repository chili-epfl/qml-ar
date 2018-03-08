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
