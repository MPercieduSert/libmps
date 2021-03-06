/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERARBRESIMPLE_H
#define MANAGERARBRESIMPLE_H

#include "AbstractManagerArbre.h"


namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbreSimple.
 */
template<class Ent> class ManagerArbreSimple : public AbstractManagerArbre<Ent> {
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;

//    using ManagerSqlEnt::m_link;
//    using ManagerSqlEnt::m_sqlAdd;
//    using ManagerSqlEnt::execFinish;

//    using ManagerSqlEnt::prepare;
//    using ManagerSqlEnt::messageErreurs;
//    using ManagerSqlEnt::messageErreursUnique;
//    using ManagerSqlEnt::modify;
//    using ManagerSqlEnt::table;

public:
    using AbstractManagerArbre<Ent>::AbstractManagerArbre;
//    using ManagerSqlEnt::attribut;
//    using ManagerSqlEnt::existsUnique;
   using ManagerSqlEnt::exists;
//    using ManagerSqlEnt::get;
    using ManagerSqlEnt::getList;
    using ManagerSqlEnt::getListId;
    using ManagerSqlEnt::name;
//    using ManagerSqlEnt::sameInBdd;
//    using ManagerSqlEnt::save;

    //! Destructeur.
    ~ManagerArbreSimple() override = default;

    //! Supprime de la base de donnée le noeud d'identifiant id ainsi que tous ses déscendants (opération stable).
    bool del(idt id) override;

    //! Renvoie le liste des descendant direct d'entity.
    VectorPtr<Ent> getListChilds(const Ent & entity) override
        {return getList(Ent::Parent,entity.id(),Ent::Ordre);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    std::list<idt> getListChildsId(idt id) override
        {return getListId(Ent::Parent,id,Ent::Ordre);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    std::vector<std::pair<idt,bool>> getListChildsIdLeaf(idt id) override {
        auto childs = getListChildsId(id);
        std::vector<std::pair<idt,bool>> liste(childs.size());
        auto j = liste.begin();
        for(auto i = childs.begin(); i != childs.end(); ++i) {
            j->first = *i;
            j->second = exists(Ent::Parent,*i);
        }
        return liste;
    }

    //! Renvoie la liste des identifiants des racines.
    std::list<idt> getListRacinesId() override
        {return getListId(Ent::Parent,QVariant(QVariant::Int),Ent::Ordre,bmps::condition::Is);}

//    //! Renvoie le vecteur des descendant direct d'entity.
//    VectorPtr<Ent> getVectorChilds(const Ent & entity) override
//        {return getListChilds(entity);}

    //! Test la validité de la paternité.
    bool isValidParent(const Ent & entity)
        {return entity.parent() == 0 || exists(Ent(entity.parent()));}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override {
        exceptionParentInvalid(entity);
        ManagerSqlEnt::save(entity);
    }

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override {
        exceptionParentInvalid(entity);
        ManagerSqlEnt::save(entity);
    }

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return bmps::ArbreSimpleTypeManager;}

protected:
    //! Lance une execption si la paternité n'est pas valide.
    void exceptionParentInvalid(const Ent & entity) {
        if(!isValidParent(entity))
            throw entityMPS::InvalideEntityException(QString("void ManagerArbreSimple<").append(name())
                                                     .append(">::exceptionParentInvalid(const ").append(name()).append(" &)")
                                                     .append("L'entité transmise en argument de getArbre n'est pas valide, "
                                                             "le parent spécifié n'existe pas."),entity);
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    virtual void saveExt(typename tree<Ent>::iterator /*iter*/, idt /*idRoot*/) override {}

    //! Sauve la racine de l'arbre (donnée et structure).
    void saveRoot(tree<Ent> & tree) override
        {save(*tree.begin());}

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds
    //! et des permutations à l'interieur de l'arbre.
    void saveWithoutDelete(typename tree<Ent>::iterator iter) override;
};

template<class Ent> bool ManagerArbreSimple<Ent>::del(idt id) {
    if(!exists(Ent::Parent,id))
        return ManagerSqlEnt::del(id);
    else
        return false;
    /*ListPtr<Ent> childs = getList(Ent::Parent,id);
    typename ListPtr<Ent>::iterator i = childs.begin();
    while(i != childs.end() && del((*i).id()))
        ++i;
    if(i == childs.end())
        return ManagerSqlEnt::del(id);
    else
        return false;*/
}

template<class Ent> void ManagerArbreSimple<Ent>::saveWithoutDelete(typename tree<Ent>::iterator iter) {
    for(auto child = iter.beginChild(); child; ++child) {
        child->setParent(iter->id());
        save(*child);
        saveWithoutDelete(child);
    }
}
}
#endif // MANAGERARBRESIMPLE_H
