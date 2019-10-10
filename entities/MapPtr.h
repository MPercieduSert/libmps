/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef MAPPTR_H
#define MAPPTR_H

#include <QMap>
#include <QMap>

#include "macrolibmps.h"

/*! \ingroup groupeDivers
 * \brief Classe patron des maps de pointeurs.
 */
template<class T> class MapPtr : public QMap<int,T*>
{
public:
    //! Itérateur.
    class iterator : public QMap<int,T*>::const_iterator
    {
    public:
        using QMap<int,T*>::const_iterator::const_iterator;

        //! Constructeur de recopie.
        iterator(const typename QMap<int,T*>::const_iterator & i) : QMap<int,T*>::const_iterator(i) {}

        //! Constructeur.
        iterator(const typename QMap<int,T*>::const_iterator && i) : QMap<int,T*>::const_iterator(i) {}

        //! Opérateur *.
        T & operator *() const
            {return *(QMap<int,T*>::const_iterator::operator *());}

        //! Opérateur [].
        T & operator [](int n) const
            {return *(QMap<int,T*>::const_iterator::operator [](n));}
    };

public:
    using QMap<int,T*>::insert;
    using QMap<int,T*>::cbegin;
    using QMap<int,T*>::cend;

    CONSTR_DEFAUT(MapPtr)

    //! Constructeur de recopie.
    MapPtr(const MapPtr<T> & liste)
    {
        for(typename QMap<int,T*>::const_iterator i = liste.cbegin(); i != liste.cend(); ++i)
            insert(i.key(),new T(**i));
    }

    //! Constructeur de déplacement.
    MapPtr(MapPtr<T> && ) = default;

    //! Destructeur.
    ~MapPtr()
    {
        for(typename QMap<int,T*>::const_iterator i = cbegin(); i != cend(); ++i)
            delete *i;
    }

    //! Renvoie un itérateur sur le début de la liste.
    iterator begin() const
        {return QMap<int,T*>::cbegin();}


    //! Renvoie un itérateur sur la fin de la liste.
    iterator end() const
        {return QMap<int,T*>::cend();}

    //! Supprime toutes les entités pointées par les pointeurs de la liste.
    void clear()
    {
        for(typename QMap<int,T*>::const_iterator i = cbegin(); i != cend(); ++i)
            delete *i;
        QMap<int,T*>::clear();
    }

    //! Supprime la liste des pointeurs, sans supprimer les entités pointés par les pointeurs composants cette liste.
    void clearList()
        {QMap<int,T*>::clear();}

    //! Insert un pointeur dans la liste.
    void insert(int key, const T & entity)
        {insert(key, new T(entity));}

    //! Insert un pointeur dans la liste.
    void insert(const T & entity)
        {insert(entity.id(), entity);}

    //! Insert un pointeur dans la liste.
    void insert(T * ptr)
        {insert(ptr->id(), ptr);}

    //! Retourne une référence constante sur l'entité de clé n.
    const T & value(int n) const
        {return *(value(n,0));}

    //! Affectation par copie
    MapPtr<T> & operator = (const MapPtr<T> & liste)
    {
        clear();
        for(typename QMap<int,T*>::const_iterator i = liste.cbegin(); i != liste.cend(); ++i)
            insert(i.key(),new T(**i));
        return *this;
    }

    //! Affectation par déplacement
    MapPtr<T> & operator = (MapPtr<T> && ) = default;

    //! Opérateur [].
    T & operator [](int n) const
        {return *(QMap<int,T*>::operator [](n));}
};

#endif // MAPPTR_H
