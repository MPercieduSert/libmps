/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef MAPPTR_H
#define MAPPTR_H

#include <map>
#include "macrolibmps.h"

namespace conteneurMPS {

/*! \ingroup groupeConteneur
 * \brief Classe patron des maps de pointeurs.
 */
template<class T> class MapPtr : public std::map<int,T*>
{
protected:
    using Mptr = std::map<int,T*>;

    /*! \ingroup groupeConteneur
     * \brief Itérateur constant.
     */
    class const_iterator : public Mptr::const_iterator {
    protected:
        using MptrIter = typename Mptr::const_iterator;

    public:
        //! Constructeur.
        const_iterator(const MptrIter & iter) noexcept : MptrIter(iter) {}

        //! Opérator *
        std::pair<const int, const T &> operator *() const noexcept {
            auto & iter = MptrIter::operator*();
            return std::pair<const int, const T &>(iter.frist,*iter.second);}

        //! Opérator ->.
        T const * operator -> () const noexcept
            {return MptrIter::operator*();}
    };

    /*! \ingroup groupeConteneur
     * \brief Itérateur.
     */
    class iterator : public Mptr::const_iterator {
    protected:
        using MptrIter = typename Mptr::const_iterator;

    public:
        //! Constructeur.
        iterator(const MptrIter & iter) noexcept : MptrIter(iter) {}

        //! Opérator *
        std::pair<const int, const T &> operator *() const noexcept {
            auto & iter = MptrIter::operator*();
            return std::pair<const int, const T &>(iter.frist,*iter.second);}

        //! Opérator *
        std::pair<const int, T &> operator *() noexcept {
            auto & iter = MptrIter::operator*();
            return std::pair<const int, T &>(iter.frist,*iter.second);}

        //! Opérator ->.
        T const * operator -> () const noexcept
            {return MptrIter::operator*();}

        //! Opérator ->.
        T * operator -> () noexcept
            {return MptrIter::operator*();}
    };

    using Mptr::empty;
    using Mptr::size;

public:
    using Mptr::insert;

    MapPtr() = default;

    //! Constructeur de recopie.
    MapPtr(const MapPtr<T> & map) {
        for(auto i = map.cbegin(); i != map.cend(); ++i)
            insert(i-, new T(i));
    }

    //! Constructeur de déplacement.
    MapPtr(MapPtr<T> && ) = default;

//    //! Destructeur.
//    ~MapPtr()
//    {
//        for(typename Mptr::const_iterator i = cbegin(); i != cend(); ++i)
//            delete *i;
//    }

    //! Renvoie un itérateur sur le début de la liste.
    iterator begin() const
        {return Mptr::cbegin();}


    //! Renvoie un itérateur constant sur le début de la liste.
    const_iterator cbegin() const
        {return Mptr::cbegin();}

    //! Renvoie un itérateur sur la fin de la liste.
    iterator end() const
        {return Mptr::cend();}

    //! Renvoie un itérateur constant sur la fin de la liste.
    const_iterator cend() const
        {return Mptr::cend();}

//    //! Supprime toutes les entités pointées par les pointeurs de la liste.
//    void clear()
//    {
//        for(typename Mptr::const_iterator i = cbegin(); i != cend(); ++i)
//            delete *i;
//        Mptr::clear();
//    }

//    //! Supprime la liste des pointeurs, sans supprimer les entités pointés par les pointeurs composants cette liste.
//    void clearList()
//        {Mptr::clear();}

    //! Insert un pointeur dans la liste.
    void insert(int key, const T & entity)
        {insert(key, std::make_unique<T>(entity));}

    //! Insert un pointeur dans la liste.
    void insert(const T & entity)
        {insert(entity.id(), entity);}

    //! Insert un pointeur dans la liste.
    void insert(std::unique_ptr<T> ptr)
        {insert(ptr->id(), ptr);}

//    //! Retourne une référence constante sur l'entité de clé n.
//    const T & value(int n) const
//        {return *(value(n,0));}

    //! Affectation par copie
    MapPtr<T> & operator = (const MapPtr<T> & liste)
    {
        Mptr::clear();
        for(auto i = liste.cbegin(); i != liste.cend(); ++i)
            insert(i.key(),std::make_unique<T>(**i));
        return *this;
    }

    //! Affectation par déplacement
    MapPtr<T> & operator = (MapPtr<T> && ) = default;

    //! Opérateur [].
    T & operator [](int n) const
        {return *(Mptr::operator [](n));}
};
}
#endif // MAPPTR_H
