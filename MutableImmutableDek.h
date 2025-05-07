#ifndef MUTABLEIMMUTABLEDEK_H
#define MUTABLEIMMUTABLEDEK_H
#include "Deck.h"
template <class T>
class MutableDeck : public SegmentDek<T> {
public:
    using SegmentDek<T>::SegmentDek;

    SegmentDek<T>* Instance() override { return this; }
    SegmentDek<T>* Clone()   const override { return new MutableDeck<T>(*this); }
};

template <class T>
class ImmutableDeck : public SegmentDek<T> {
public:
    using SegmentDek<T>::SegmentDek;

    SegmentDek<T>* Instance() override { return this->Clone(); }
    SegmentDek<T>* Clone()   const override { return new ImmutableDeck<T>(*this); }
};


#endif //MUTABLEIMMUTABLEDEK_H
