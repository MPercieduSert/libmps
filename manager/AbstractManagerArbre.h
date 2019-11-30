/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/11/2019
 */
#ifndef ABSTRACTMANAGERARBRE_H
#define ABSTRACTMANAGERARBRE_H

#include "ManagerSql.h"
#include "Tree.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template abstraite parent des différents manageurs pour les entités de type arbre.
 */
template<class Ent> class AbstractManagerArbre : public virtual ManagerSql<Ent> {
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;
    using ManagerSqlEnt::del;
public:
    using ManagerSqlEnt::ManagerSql;
    using ManagerSqlEnt::getListChildsId;
    using ManagerSqlEnt::get;
    using ManagerSqlEnt::save;

    //! Renvoie l'arbre de toutes les entités.
    tree<Ent> getArbre() override {
        auto racines = getListRacinesId();
        tree<Ent> tree;
        auto iter = tree.begin();
        for(auto i = racines.cbegin(); i != racines.cend(); ++i)
            get(*tree.push_back(iter,Ent(*i)));
        while(iter){
            if(!optiLeaf(iter->id())){
                auto listChilds = getListChildsId(iter->id());
                for(auto i = listChilds.cbegin(); i != listChilds.cend(); ++i)
                    get(*tree.push_back(iter,Ent(*i)));
            }
            ++iter;
        }
        return tree;
    }

    //! Renvoie l'arbre de racine entity.
    tree<Ent> getArbre(const Ent & entity) override
        {return getArbre(entity.id());}

    //! Renvoie l'arbre l'entity d'identifiant.
    tree<Ent> getArbre(idt id) override {
        Ent ent(id);
        if(get(ent)) {
            tree<Ent> tree(ent);
            auto iter = tree.begin();
            while(iter){
                if(!optiLeaf(iter->id())){
                    auto listChilds = getListChildsId(iter->id());
                    for(auto i = listChilds.cbegin(); i != listChilds.cend(); ++i)
                        get(*tree.push_back(iter,Ent(*i)));
                }
                ++iter;
            }
            return tree;
        }
        else
            throw std::invalid_argument("L'entité transmise en argument de getArbre n'existe pas.");
    }

    //! Renvoie la liste des identifiants des racines.
    virtual std::list<idt> getListRacinesId() = 0;

    //! Enregistre l'arbre d'entités.
    void save(tree<Ent> & tree, bmps::treeSave n = bmps::treeSave::ExternalChange) override;

protected:
    //! Supprime de la Base de données les noeuds hors de l'arbre.
    virtual void deleteLeafOutOf(typename tree<Ent>::iterator iter);

    //! Teste si le noeud d'identifiant id est une feuille dans la base de donnée en vue d'optimisation.
    virtual bool optiLeaf(idt /*id*/)
        {return false;}

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds.
    virtual void saveAddLeaf(typename tree<Ent>::iterator iter)
        {saveWithoutDelete(iter);}

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    virtual void saveExt(typename tree<Ent>::iterator iter, idt idRoot) = 0;

    //! Sauve la racine de l'arbre (donnée et structure).
    virtual void saveRoot(tree<Ent> & tree) = 0;

    //! Sauve un arbre où le changement de structure consite seulement
    //! l'ajout de nouveaux noeuds et des permutations à l'interieur de l'arbre.
    virtual void saveWithoutDelete(typename tree<Ent>::iterator iter) = 0;
};

template<class Ent> void AbstractManagerArbre<Ent>::deleteLeafOutOf(typename tree<Ent>::iterator iter) {
    while(iter) {
        if(iter.leaf()) {
            auto childs = getListChildsId(iter->id());
            for(auto i = childs.cbegin(); i != childs.cend(); ++i)
                del(*i);
        }
//        else {
//            auto childs = getListChildsId(iter->id());
//            for(auto i = childs.cbegin(); i != childs.cend(); ++i){
//                if(i->num() < 0 || i->num() >= iter->sizeChild())
//                    del(*i);
//            }
//        }
        ++iter;
    }
}

template<class Ent> void AbstractManagerArbre<Ent>::save(tree<Ent> & tree, bmps::treeSave n) {
    using namespace bmps;
    if(n == treeSave::EntityOnly) {
        for(auto i = tree.begin(); i ; ++i)
            save(*i);
    }
    else if(n == treeSave::EntityOnlyWhitoutRoot) {
        auto i = tree.begin();
        ++i;
        while (i) {
            save(*i);
            ++i;
        }
    }
    else {
        if(n < treeSave::EntityOnlyWhitoutRoot)
            saveRoot(tree);
        switch (n) {
        case treeSave::AddLeaf:
        case treeSave::AddLeafWhitoutRoot:
            saveAddLeaf(tree.begin());
            break;

        case treeSave::WithoutDelete:
        case treeSave::WithoutDeleteWhitoutRoot:
            saveWithoutDelete(tree.begin());
            break;

        case treeSave::InternalChange:
        case treeSave::InternalChangeWhitoutRoot:
            saveWithoutDelete(tree.begin());
            deleteLeafOutOf(tree.begin());
            break;

        case treeSave::ExternalChange:
        case treeSave::ExternalChangeWhitoutRoot:
            saveExt(tree.begin(),tree.begin()->id());
            deleteLeafOutOf(tree.begin());
            break;

        case treeSave::EntityOnly:
        case treeSave::EntityOnlyWhitoutRoot:
            break;
        }
    }
}
}
#endif // ABSTRACTMANAGERARBRE_H
