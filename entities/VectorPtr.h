/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/09/2016
 */
#ifndef VECTORPTR_H
#define VECTORPTR_H

#include <QList>
#include <QVector>

#include "ListPtr.h"

namespace conteneurMPS {

/*! \ingroup groupeConteneur
 * \brief Classe patron des vector de pointeurs.
 */
template<class T> class VectorPtr : public QVector<std::unique_ptr<T>>
{
protected:
    using Luptr = QList<std::unique_ptr<T>>;
    using Vuptr = QVector<std::unique_ptr<T>>;
public:
    //! Itérateur.
    class iterator
    {
    protected:
        //! Type de l'itérateur sur un pointeur.
        using iter = std::unique_ptr<T> const *;
        iter m_ptr;     //!< Pointeur de l'itérateur.

    public:
        //! Constructeur.
        iterator(iter ptr = nullptr)
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

    using Vuptr::append;
    using Vuptr::begin;
    using Vuptr::cbegin;
    using Vuptr::cend;
    using Vuptr::end;
    using Vuptr::resize;
    using Vuptr::size;


    //! Constructeur par defaut.
    VectorPtr() = default;

    //! Constructeur de recopie.
    VectorPtr(const VectorPtr<T> & vect)
        : Vuptr(vect.size())
    {
        auto j = begin();
        for(auto i = vect.cbegin(); i != vect.cend(); ++i, ++j)
            *j = std::make_unique<T>(**i);
    }

    //! Constructeur de déplacement
    VectorPtr(VectorPtr<T> && vect) = default;

    //! Constructeur à partir d'une liste d'entités.
    VectorPtr(const ListPtr<T> & liste);

    //! Constructeur à partir d'une liste d'entités temporaire.
    VectorPtr(ListPtr<T> && liste);

    //! Constructeur
    VectorPtr(int n,const T & value = T()) : Vuptr(n)
    {
        for(auto i = begin(); i != end(); ++i)
            *i = std::make_unique<T>(value);
    }

//    //! Destructeur.
//    ~VectorPtr()
//    {
//        for(typename Vuptr::const_iterator i = cbegin(); i != cend(); ++i)
//            delete *i;
//    }

    //! Ajoute un élément à la liste.
    void append(const T & value)
        {append(std::make_unique<T>(value));}

    //! Renvoie une référence sur l'entité d'identifiant id si elle existe.
    iterator findId(unsigned id);

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
        {return *(Vuptr::operator[](n));}

    const T & operator[](int n) const
        {return *(Vuptr::operator[](n));}
};

template<class T> typename VectorPtr<T>::iterator VectorPtr<T>::findId(unsigned id)
{
    iterator i = begin();
    while(i != end() && (*i).id() != id)
        ++i;
    return i;
}

template<class T> VectorPtr<T>::VectorPtr(const ListPtr<T> & liste)
    : Vuptr(liste.size())
{
    if(!liste.isEmpty())
    {
        auto i = begin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = std::make_unique<T>(**j);
    }
}

template<class T> VectorPtr<T>::VectorPtr(ListPtr<T> && liste)
    : Vuptr(liste.size())
{
    if(!liste.isEmpty())
    {
        auto i = begin();
        for(auto j = liste.Luptr::begin(); j != liste.Luptr::end(); ++i, ++j)
            *i = std::move(*j);
        liste.clear();
    }
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const ListPtr<T> & liste)
{
    if(!liste.isEmpty())
    {
        resize(liste.size());
        Vuptr::squeeze();
        auto i = begin();
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = std::make_unique<T>(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(ListPtr<T> && liste)
{
    if(!liste.isEmpty())
    {
        resize(liste.size());
        Vuptr::squeeze();
        auto i = begin();
        for(auto j = liste.Luptr::begin(); j != liste.Luptr::end(); ++i, ++j)
            *i = std::move(*j);
        liste.clear();
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(const VectorPtr<T> & vector)
{
    if(!vector.isEmpty())
    {
        resize(vector.size());
        Vuptr::squeeze();
        auto i = begin();
        for(auto j = vector.cbegin(); j != vector.cend(); ++j, ++i)
            *i = std::make_unique<T>(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> & VectorPtr<T>::operator =(VectorPtr<T> && vector)
{
    if(!vector.isEmpty())
    {
        resize(vector.size());
        Vuptr::squeeze();
        auto i = begin();
        for(auto j = vector.begin(); j != vector.end(); ++j, ++i)
            *i = std::move(*j);
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
        auto i = begin() + n;
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = std::make_unique<T>(**j);
    }
    return *this;
}

template<class T> VectorPtr<T> &VectorPtr<T>::operator <<(ListPtr<T> && liste)
{
    if(!liste.isEmpty())
    {
        int n = size();
        resize(n + liste.size());
        auto i = begin() + n;
        for(auto j = liste.cbegin(); j != liste.cend(); ++i, ++j)
            *i = *j;
        liste.clear();
    }
    return *this;
}
}
#endif // VECTORPTR_H
