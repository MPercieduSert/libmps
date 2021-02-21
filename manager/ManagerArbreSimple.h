/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERARBRESIMPLE_H
#define MANAGERARBRESIMPLE_H

#include "AbstractManagerarbre.h"


namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbreSimple.
 */
template<class Ent> class ManagerarbreSimple : public AbstractManagerarbre<Ent> {
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
    using AbstractManagerarbre<Ent>::AbstractManagerarbre;
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
    ~ManagerarbreSimple() override = default;

    //! Supprime de la base de donnée le noeud d'identifiant id ainsi que tous ses déscendants (opération stable).
    bool del(idt id) override;

    //! Renvoie le liste des descendant direct d'entity.
    vector_ptr<Ent> getListChilds(const Ent &ent) override
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
//    vector_ptr<Ent> getVectorChilds(const Ent &ent) override
//        {return getListChilds(entity);}

    //! Test la validité de la paternité.
    bool is_validParent(const Ent &ent)
        {return entity.parent() == 0 || exists(Ent(entity.parent()));}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override {
        exceptionParentInvalid(entity);
        ManagerSqlEnt::save(entity);
    }

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override {
        exceptionParentInvalid(entity);
        ManagerSqlEnt::save(entity);
    }

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return bmps::arbreSimpleTypeManager;}

protected:
    //! Lance une execption si la paternité n'est pas valide.
    void exceptionParentInvalid(const Ent &ent) {
        if(!is_validParent(entity))
            throw entityMPS::invalide_entity_exception(QString("void ManagerarbreSimple<").append(name())
                                                     .append(">::exceptionParentInvalid(const ").append(name()).append(" &)")
                                                     .append("L'entité transmise en argument de getarbre n'est pas valide, "
                                                             "le parent spécifié n'existe pas."),entity);
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    virtual void saveExt(typename tree<Ent>::iterator /*iter*/, idt /*idRoot*/) override {}

    //! Sauve la racine de l'arbre (donnée et structure).
    void saveRoot(tree<Ent> &tree) override
        {save(*tree.begin());}

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds
    //! et des permutations à l'interieur de l'arbre.
    void saveWithoutDelete(typename tree<Ent>::iterator iter) override;
};

template<class Ent> bool ManagerarbreSimple<Ent>::del(idt id) {
    if(!exists(Ent::Parent,id))
        return ManagerSqlEnt::del(id);
    else
        return false;
    /*list_ptr<Ent> childs = getList(Ent::Parent,id);
    typename list_ptr<Ent>::iterator i = childs.begin();
    while(i != childs.end() &&del((*i).id()))
        ++i;
    if(i == childs.end())
        return ManagerSqlEnt::del(id);
    else
        return false;*/
}

template<class Ent> void ManagerarbreSimple<Ent>::saveWithoutDelete(typename tree<Ent>::iterator iter) {
    for(auto child = iter.begin_child(); child; ++child) {
        child->set_parent(iter->id());
        save(*child);
        saveWithoutDelete(child);
    }
}
}
#endif // MANAGERARBRESIMPLE_H
