/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/03/2018
 */
#ifndef LISTPTR_H
#define LISTPTR_H

#include <QList>
#include <QVector>

#include "macrolibmps.h"


/*! \defgroup groupeDivers Divers
 * \brief Ensemble de classes diverses.
 */

/*! \ingroup groupeDivers
 * \brief Classe patron des listes de pointeurs.
 */
template<class T> class ListPtr : public QList<T*>
{
public:
    //! Itérateur.
    class iterator : public QList<T*>::const_iterator
    {
    public:
        using QList<T*>::const_iterator::const_iterator;

        //! Constructeur de recopie.
        iterator(const typename QList<T*>::const_iterator & i) : QList<T*>::const_iterator(i) {}

        //! Constructeur.
        iterator(const typename QList<T*>::const_iterator && i) : QList<T*>::const_iterator(i) {}

        //! Opérateur *.
        T & operator *() const
            {return *(QList<T*>::const_iterator::operator *());}

        //! Opérateur [].
        T & operator [](int n) const
            {return *(QList<T*>::const_iterator::operator [](n));}
    };

public:
    using QList<T*>::append;
    using QList<T*>::cbegin;
    using QList<T*>::cend;
    using QList<T*>::operator <<;

    CONSTR_DEFAUT(ListPtr)

    //! Constructeur de recopie.
    ListPtr(const ListPtr<T> & liste)
    {
        reserve(liste.size());
        for(typename QList<T*>::const_iterator i = liste->cbegin(); i != liste->cend(); ++i)
            append(new T(**i));
    }

    //! Constructeur de déplacement.
    ListPtr(ListPtr<T> && ) = default;

    //! Destructeur.
    ~ListPtr()
    {
        for(typename QList<T*>::const_iterator i = cbegin(); i != cend(); ++i)
            delete *i;
    }

    //! Ajoute un pointeur à la fin de la liste.
    void append(const T & entity)
        {append(new T(entity));}

    //! Retourne une référence constante sur la n-ième entité de la listes.
    const T & at(int n) const
        {return *(at(n));}

    //! Renvoie un itérateur sur le début de la liste.
    iterator begin() const
        {return QList<T*>::cbegin();}

    //! Renvoie un itérateur sur la fin de la liste.
    iterator end() const
        {return QList<T*>::cend();}

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear()
    {
        for(typename QList<T*>::const_iterator i = cbegin(); i != cend(); ++i)
            delete *i;
        QList<T*>::clear();
    }

    //! Supprime la liste des pointeurs, sans supprimer les entités pointés par les pointeurs composants cette liste.
    void clearList()
        {QList<T*>::clear();}

    //! Affectation par copie
    ListPtr<T> & operator = (const ListPtr<T> & liste)
    {
        clear();
        if(!liste.isEmpty())
        {
            reserve(liste.size());
            for(typename QList<T*>::const_iterator i = liste->cbegin(); i != liste->cend(); ++i)
                append(new T(**i));
        }
        return *this;
    }

    //! Affectation par déplacement
    ListPtr<T> & operator = (ListPtr<T> && liste)
    {
        QList<T*>::operator =(liste);
        liste.clearList();
        return *this;
    }

    //! Ajoute un pointeur sur une entité à la liste (ce pointeur doit être créer dynamiquement).
    ListPtr<T> & operator << (T * entity)
        {append(entity); return *this;}
};

#endif // LISTPTR_H
