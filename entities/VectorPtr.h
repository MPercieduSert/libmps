/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/09/2016
 */
#ifndef VECTORPTR_H
#define VECTORPTR_H

#include <QList>
#include <QVector>

#include "ListPtr.h"

/*! \ingroup groupeDivers
 * \brief Classe patron des vector de pointeurs.
 */
template<class T> class VectorPtr : public QVector<T*>
{
public:
    //! Itérateur.
    class iterator
    {
    protected:
        //! Type de l'itérateur sur un pointeur.
        typedef T * const * iter;
        iter m_ptr;     //!< Pointeur de l'itérateur.

    public:
        //! Constructeur.
        iterator(iter ptr = 0)
            : m_ptr(ptr)
        {}

        //! Constructeur.
        iterator(const iterator & i)
            : m_ptr(i.m_ptr)
        {}

        //! Opérator d'affectation
        iterator & operator =(const iterator & i)
            {m_ptr = i.m_ptr; return *this;}

        //! Opérator d'affectation
        iterator & operator =(iter ptr)
            {m_ptr = ptr; return *this;}

        //! Opérator *
        T & operator *() const
            {return *(*m_ptr);}

        //! Opérateur de pré-incrémentation
        iterator &  operator ++()
            {++m_ptr; return *this;}

        //! Opérateur de post-incrémentation
        iterator operator ++(int /*n*/)
            {iterator i = *this; ++m_ptr; return i;}

        //! Opérateur de pré-décrémentation
        iterator &  operator --()
            {--m_ptr; return *this;}

        //! Opérateur de post-incrémentation
        iterator operator --(int /*n*/)
            {iterator i = *this; --m_ptr; return i;}

        //! Test d'égalité entre deux itérateurs.
        bool operator ==(const iterator & i) const
            {return m_ptr == i.m_ptr;}

        //! Test de différence entre deux itérateurs.
        bool operator !=(const iterator & i) const
            {return m_ptr != i.m_ptr;}

        //! Incrémente n fois l'itérateur.
        iterator & operator +=(int n)
            {m_ptr += n; return *this;}

        //! Décrémente n fois l'itérateur.
        iterator & operator -=(int n)
            {m_ptr -= n; return *this;}

        //! L'addition d'un itérateur i et d'un entier n produit une copie de l'itérateur i incrémenté n fois.
        iterator operator +(int n) const
        {
            iterator i(*this);
            return i += n;
        }

        //! La soustraction d'un itérateur i et d'un entier n produit une copie de l'itérateur i décrémenté n fois.
        iterator operator -(int n) const
        {
            iterator i(*this);
            return i -= n;
        }
    };

    using QVector<T*>::append;
    using QVector<T*>::begin;
    using QVector<T*>::cbegin;
    using QVector<T*>::cend;
    using QVector<T*>::end;
    using QVector<T*>::resize;
    using QVector<T*>::size;


    //! Constructeur par defaut.
    VectorPtr() = default;

    //! Constructeur de recopie.
    VectorPtr(const VectorPtr<T> & vect)
        : QVector<T*>(vect.size())
    {
        typename QVector<T*>::iterator j = begin();
        for(typename QVector<T*>::const_iterator i = vect.cbegin(); i != vect.cend(); ++i, ++j)
            *j = new T(**i);
    }

    //! Constructeur de déplacement
    VectorPtr(VectorPtr<T> && vect) = default;

    //! Constructeur à partir d'une liste d'entités.
    VectorPtr(const ListPtr<T> & liste);

    //! Constructeur à partir d'une liste d'entités temporaire.
    VectorPtr(ListPtr<T> && liste);

    //! Constructeur
    VectorPtr(int n,const T & value = T()) : QVector<T*>(n)
    {
        for(typename QVector<T*>::iterator i = begin(); i != end(); ++i)
            *i = new T(value);
    }

    //! Destructeur.
    ~VectorPtr()
    {
        for(typename QVector<T*>::const_iterator i = cbegin(); i != cend(); ++i)
            delete *i;
    }

    //! Ajoute un élément à la liste.
    void append(const T & value)
        {append(new T(value));}

    //! Renvoie une référence sur l'entité d'identifiant id si elle existe.
    iterator findId(int id);

    //! Affectation par copie d'une liste de type ListeEntities.
    VectorPtr<T> &operator =(const ListPtr<T> & liste);

    //! Affectation par déplacement du liste (temporaire) de type ListeEntities.
    VectorPtr<T> &operator =(ListPtr<T> && liste);

    //! Affectation par recopie.
    VectorPtr<T> &operator =(const VectorPtr<T> & vector);

    //! Affectation par déplacement.
    VectorPtr<T> &operator =(VectorPtr<T> && vector);

    //! Ajoute les éléments de liste à la suite de la liste.
    VectorPtr<T> &operator <<(const ListPtr<T> & liste);

    //! Ajoute les éléments de liste (temporaire) à la suite de la liste.
    VectorPtr<T> &operator <<(ListPtr<T> && liste);

    T & operator[](int n)
        {return *(QVector<T*>::operator[](n));}

    const T & operator[](int n) const
        {return *(QVector<T*>::operator[](n));}
};

template<class T> typename VectorPtr<T>::iterator VectorPtr<T>::findId(int id)
{
    iterator i = begin();
    while(i != end() && (*i).id() != id)
    {
        ++i;
    }
    return i;
}

template<class T> VectorPtr<T>::VectorPtr(const ListPtr<T> & liste)
    : QVector<T*>(liste.size())
{
    if(!liste.isEmpty())
    {
        typename QVector<T*>::iterator i = begin();
        for(typename QList<T*>::const_iterator j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
}

template<class T> VectorPtr<T>::VectorPtr(ListPtr<T> && liste)
    : QVector<T*>(liste.size())
{
    if(!liste.isEmpty())
    {
        typename QVector<T*>::iterator i = begin();
        for(typename QList<T*>::const_iterator j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
    }
    liste.clearList();
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const ListPtr<T> & liste)
{
    for(typename QVector<T*>::const_iterator i = cbegin(); i != cend(); ++i)
        delete *i;
    QVector<T*>::clear();
    if(!liste.isEmpty())
    {
        resize(liste.size());
        typename QVector<T*>::iterator i = begin();
        for(typename QList<T*>::const_iterator j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(ListPtr<T> && liste)
{
    for(typename QVector<T*>::const_iterator i = cbegin(); i != cend(); ++i)
        delete *i;
    QVector<T*>::clear();
    if(!liste.isEmpty())
    {
        resize(liste.size());
        typename QVector<T*>::iterator i = begin();
        for(typename QList<T*>::const_iterator j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
    }
    liste.clearList();
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const VectorPtr<T> & vector)
{
    for(typename QVector<T*>::const_iterator i = cbegin(); i != cend(); ++i)
        delete *i;
    QVector<T*>::clear();
    if(!vector.isEmpty())
    {
        resize(vector.size());
        typename QVector<T*>::iterator i = begin();
        for(typename QVector<T*>::const_iterator j = vector.cbegin(); j != vector.cend(); ++j, ++i)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(VectorPtr<T> && vector)
{
    for(typename QVector<T*>::const_iterator i = cbegin(); i != cend(); ++i)
        delete *i;
    QVector<T*>::clear();
    if(!vector.isEmpty())
    {
        resize(vector.size());
        typename QVector<T*>::iterator i = begin();
        for(typename QVector<T*>::const_iterator j = vector.cbegin(); j != vector.cend(); ++j, ++i)
            *i = *j;
        vector.clear();
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator << (const ListPtr<T> & liste)
{
    if(!liste.isEmpty())
    {
        int n = size();
        resize(n + liste.size());
        typename QVector<T*>::iterator i = begin() + n;
        for(typename QList<T*>::const_iterator j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = new T(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> &VectorPtr<T>::operator <<(ListPtr<T> && liste)
{
    if(!liste.isEmpty())
    {
        int n = size();
        resize(n + liste.size());
        typename QVector<T*>::iterator i = begin() + n;
        for(typename QList<T*>::const_iterator j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
    }
    liste.clearList();
    return *this;
}
#endif // VECTORPTR_H
