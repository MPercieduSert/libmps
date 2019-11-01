/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERARBRESIMPLE_H
#define MANAGERARBRESIMPLE_H

#include "ManagerArbre.h"
#include "ManagerSql.h"
#include "Tree.h"

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbreSimple.
 */
template<class Ent> class ManagerArbreSimple : public virtual ManagerSql<Ent>
{
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;

    using ManagerSqlEnt::m_link;
    using ManagerSqlEnt::m_sqlAdd;
    using ManagerSqlEnt::execFinish;

    using ManagerSqlEnt::prepare;
    using ManagerSqlEnt::messageErreurs;
    using ManagerSqlEnt::messageErreursUnique;
    using ManagerSqlEnt::modify;
    using ManagerSqlEnt::table;

public:
    using ManagerSqlEnt::ManagerSql;
    using ManagerSqlEnt::attribut;
    using ManagerSqlEnt::existsUnique;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::get;
    using ManagerSqlEnt::getList;
    using ManagerSqlEnt::getListId;
    using ManagerSqlEnt::sameInBdd;
    using ManagerSqlEnt::save;

    //! Destructeur.
    ~ManagerArbreSimple() override = default;

    //! Supprime de la base de donnée le noeud d'identifiant id ainsi que tous ses déscendants (opération stable).
    bool del(idt id) override;

    //! Renvoie l'arbre de toutes les entités.
    Tree<Ent> getArbre() override
    {
        auto racines = getList(Ent::Parent,QVariant(QVariant::Int),Ent::Ordre,bdd::Condition::Is);
        Tree<Ent> tree;
        for(auto i = racines.begin(); i != racines.end(); ++i)
            tree << getArbre((*i).id());
        return tree;
    }

    //! Renvoie l'arbre racine entity.
    Tree<Ent> getArbre(const Ent & entity) override
    {
        Ent ent(entity.id());
        if(get(ent))
        {
            Tree<Ent> tree(ent);
            getArbreRec(tree.root());
            return tree;
        }
        else
            throw std::invalid_argument("L'entité transmise en argument de getArbre n'est pas valide.");
    }

    //! Renvoie l'arbre l'entity d'identifiant.
    Tree<Ent> getArbre(idt id) override
        {return getArbre(Ent(id));}

    //! Renvoie le liste des descendant direct d'entity.
    ListPtr<Ent> getListChilds(const Ent & entity) override
        {return getList(Ent::Parent,entity.id(),Ent::Ordre);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    QList<idt> getListChildsId(idt id) override
        {return getListId(Ent::Parent,id,Ent::Ordre);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    QList<QPair<idt,bool>> getListChildsIdLeaf(idt id, bool ordre = true) override
    {
        auto childs = getListId(Ent::Parent,id,Ent::Ordre,ordre);
        QList<QPair<idt,bool>> liste;
        for(auto i = childs.begin(); i != childs.end(); ++i)
            liste.append(QPair<int,bool>(*i, !exists(Ent::Parent,*i)));
        return liste;
    }

    //! Renvoie le vecteur des descendant direct d'entity.
    VectorPtr<Ent> getVectorChilds(const Ent & entity) override
        {return getListChilds(entity);}

    //! Test la validité de la paternité.
    bool isValidParent(const Ent & entity)
        {return entity.parent() == 0 || exists(Ent(entity.parent()));}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
    {
        exceptionParentInvalid(entity);
        ManagerSqlEnt::save(entity);
    }

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
    {
        exceptionParentInvalid(entity);
        ManagerSqlEnt::save(entity);
    }

    //! Enregistre l'arbre d'entités.
    void save(Tree<Ent> & tree, bdd::TreeSave n = bdd::TreeSave::ExternalChange) override;

protected:
    //! Supprime de la Base de données les noeuds hors de l'arbre.
    void deleteLeafOutOf(TreeItem<Ent> * tree);

    //! Lance une execption si la paternité n'est pas valide.
    void exceptionParentInvalid(const Ent & entity)
    {
        if(!isValidParent(entity))
            throw std::invalid_argument(QString("L'entité transmise en argument de getArbre n'est pas valide, "
                                                "le parent spécifié n'existe pas:\n").append(entity.affiche()).toStdString());
    }

    //! Permet de lire réccursivement un arbre.
    void getArbreRec(TreeItem<Ent> * tree);

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds
    //! et des permutations à l'interieur de l'arbre.
    void saveWithoutDelete(TreeItem<Ent> * tree);
};

template<class Ent> bool ManagerArbreSimple<Ent>::del(idt id)
{
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

template<class Ent> void ManagerArbreSimple<Ent>::deleteLeafOutOf(TreeItem<Ent> * tree)
{
    if(tree->hasChild())
    {
        ListPtr<Ent> childs = getList(Ent::Parent,tree->data().id(),Ent::Ordre);
        for(typename ListPtr<Ent>::iterator j = childs.begin(); j != childs.end(); ++j)
        {
            typename QList<TreeItem<Ent>*>::const_iterator i = tree->childs().cbegin();
            while(i != tree->childs().cend() && (*j).id() != (*i)->data().id())
                ++i;
            if(i == tree->childs().cend())
                del((*j).id());
            for(i = tree->childs().cbegin(); i != tree->childs().cend(); ++i)
                deleteLeafOutOf(*i);
        }
    }
    else
    {
        ListPtr<Ent> childs = getList(Ent::Parent,tree->data().id());
        for(typename ListPtr<Ent>::iterator i = childs.begin(); i != childs.end(); ++i)
            del((*i).id());
    }
}

template<class Ent> void ManagerArbreSimple<Ent>::getArbreRec(TreeItem<Ent> * tree)
{
    ListPtr<Ent> childs = getList(Ent::Parent,tree->data().id(),Ent::Ordre);
    if(!childs.isEmpty())
    {
        for(typename ListPtr<Ent>::iterator i = childs.begin(); i != childs.end(); ++i)
        {
            TreeItem<Ent> * child = tree->addChild(Ent((*i).id()));
            get(child->modifData());
            getArbreRec(child);
        }
    }
}

template<class Ent> void ManagerArbreSimple<Ent>::save(Tree<Ent> & tree, bdd::TreeSave n)
{
    using namespace bdd;
    if(n == TreeSave::EntityOnly)
    {
        for(typename TreeItem<Ent>::iterator i = tree.begin(); i != tree.end(); ++i)
            save((*i)->data());
    }
    else if(n == TreeSave::EntityOnlyWhitoutRoot)
    {
        typename TreeItem<Ent>::iterator i = tree.begin();
        ++i;
        for(; i != tree.end(); ++i)
            save((*i)->data());
    }
    else
    {
        if(n < TreeSave::EntityOnlyWhitoutRoot)
            save(tree.root()->data());
        switch (n) {
        case TreeSave::AddLeaf:
        case TreeSave::AddLeafWhitoutRoot:
        case TreeSave::WithoutDelete:
        case TreeSave::WithoutDeleteWhitoutRoot:
            saveWithoutDelete(tree.root());
            break;

        case TreeSave::InternalChange:
        case TreeSave::InternalChangeWhitoutRoot:
        case TreeSave::ExternalChange:
        case TreeSave::ExternalChangeWhitoutRoot:
            saveWithoutDelete(tree.root());
            deleteLeafOutOf(tree.root());
            break;

        case TreeSave::EntityOnly:
        case TreeSave::EntityOnlyWhitoutRoot:
            break;
        }
    }
}

template<class Ent> void ManagerArbreSimple<Ent>::saveWithoutDelete(TreeItem<Ent> * tree)
{
    for(typename QList<TreeItem<Ent>*>::const_iterator child = tree->childs().cbegin(); child != tree->childs().cend(); ++child)
    {
        (*child)->modifData().setParent(tree->data().id());
        save((*child)->data());
        if((*child)->hasChild())
            saveWithoutDelete(*child);
    }
}
#endif // MANAGERARBRESIMPLE_H
