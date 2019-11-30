/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/03/2018
 */
#ifndef LISTPTR_H
#define LISTPTR_H


#include "macrolibmps.h"





//template<class T> class ListPtr : protected std::forward_list<T*>
//{
//public:
//    //! Itérateur.
//    class iterator : public std::forward_list<T*>::const_iterator
//    {
//    public:
//        using std::forward_list<T*>::const_iterator::const_iterator;

//        //! Constructeur de recopie.
//        iterator(const typename std::forward_list<T*>::const_iterator & i) : std::forward_list<T*>::const_iterator(i) {}

////        //! Constructeur.
////        iterator(const typename Luptr::const_iterator && i) : Luptr::const_iterator(i) {}

//        //! Opérateur *.
//        T & operator *() const
//            {return *(std::forward_list<T*>::const_iterator::operator *());}

//        //! Opérateur [].
//        T & operator [](int n) const
//            {return *(Luptr::const_iterator::operator [](n));}
//    };

//public:
//    using Luptr::append;
//    using Luptr::cbegin;
//    using Luptr::cend;
//    using Luptr::last;
//    using Luptr::operator <<;

//    CONSTR_DEFAUT(ListPtr)

//    //! Constructeur de recopie.
//    ListPtr(const ListPtr<T> & liste)
//    {
//        reserve(liste.size());
//        for(auto i = liste->cbegin(); i != liste->cend(); ++i)
//            append(std::make_unique<T>(**i));
//    }

//    //! Constructeur de déplacement.
//    ListPtr(ListPtr<T> && ) = default;

////    //! Destructeur.
////    ~ListPtr()
////    {
////        for(typename Luptr::const_iterator i = cbegin(); i != cend(); ++i)
////            delete *i;
////    }

//    //! Ajoute un pointeur à la fin de la liste.
//    void append(const T & entity)
//        {Luptr::append(std::make_unique<T>(entity));}

//    //! Ajoute un pointeur à la fin de la liste.
//    void append(const std::unique_ptr<T> & ptr)
//        {Luptr::append(std::make_unique<T>(*ptr));}

//    //! Ajoute un pointeur à la fin de la liste.
//    void append(std::unique_ptr<T> && ptr)
//    {
//        Luptr::append(std::unique_ptr<T>());
//        last() = std::move(ptr);
//    }

//    //! Retourne une référence constante sur la n-ième entité de la listes.
//    const T & at(int n) const
//        {return *(at(n));}

//    //! Renvoie un itérateur sur le début de la liste.
//    iterator begin() const
//        {return Luptr::cbegin();}

//    //! Renvoie un itérateur sur la fin de la liste.
//    iterator end() const
//        {return Luptr::cend();}

////    //! Supprime toutes les entités pointées par les pointeurs de la liste.
////    void clear()
////    {
////        for(auto i = cbegin(); i != cend(); ++i)
////            delete *i;
////        Luptr::clear();
////    }

////    //! Supprime la liste des pointeurs, sans supprimer les entités pointés par les pointeurs composants cette liste.
////    void clearList()
////        {Luptr::clear();}

//    //! Affectation par copie
//    ListPtr<T> & operator = (const ListPtr<T> & liste)
//    {
//        Luptr::clear();
//        if(!liste.isEmpty())
//        {
//            reserve(liste.size());
//            for(auto i = liste->cbegin(); i != liste->cend(); ++i)
//                append(std::make_unique<T>(**i));
//        }
//        return *this;
//    }

//    //! Affectation par déplacement
//    ListPtr<T> & operator = (ListPtr<T> && liste) = default;
////    {
////        Luptr::operator =(liste);
////        liste.clearList();
////        return *this;
////    }

//    //! Ajoute un pointeur sur une entité à la liste (ce pointeur doit être créer dynamiquement).
//    ListPtr<T> & operator << (std::unique_ptr<T> entity)
//        {append(entity); return *this;}
//};
}
#endif // LISTPTR_H
