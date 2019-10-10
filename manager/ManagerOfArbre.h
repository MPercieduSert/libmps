/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGEROFARBRE_H
#define MANAGEROFARBRE_H

#include "ManagerArbre.h"
#include "Tree.h"

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbre.
 */
template<class Ent> class ManagerOfArbre : public virtual ManagerSql<Ent>
{
protected:
    ManagerArbre m_managerArbre;        //!< Manager de la structure d'arbre.

    using ManagerSqlEnt = ManagerSql<Ent>;

    using ManagerSqlEnt::m_link;
    using ManagerSqlEnt::m_sqlAdd;
    using ManagerSqlEnt::execFinish;

    using ManagerSqlEnt::prepare;
    using ManagerSqlEnt::messageErreurs;
    using ManagerSqlEnt::messageErreursUnique;
    using ManagerSqlEnt::modify;
    using ManagerSqlEnt::saveUnique;
    using ManagerSqlEnt::table;

public:
    using ManagerSqlEnt::del;
    using ManagerSqlEnt::attribut;
    using ManagerSqlEnt::existsUnique;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::get;
    using ManagerSqlEnt::nbrAtt;
    using ManagerSqlEnt::sameInBdd;
    using ManagerSqlEnt::save;

    //! Constructeur.
    ManagerOfArbre(const InfoBdd & info, const InfoBdd & infoArbre, AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>());

    //! Destructeur.
    ~ManagerOfArbre() override = default;

    //! Creer la table.
    void creer() override
    {
        m_managerArbre.creer();
        ManagerSqlEnt::creer();
    }

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(int id) override
    {
        Arbre node(id);
        if(m_managerArbre.get(node))
        {
            if(node.feuille() && ManagerSqlEnt::del(id))
                return  m_managerArbre.del(id);
            else
                return false;
        }
        else
            return ManagerSqlEnt::del(id);
    }

    //! Applique la fonction fonction bool fct(int id) à chaque noeud descendant de celui
    //! d'identifiant id en commençant par les descendants.
    template<class Fct> bool foreachBeginChild(int id, const Fct & fct, bool ordre = true)
        {return m_managerArbre.foreachBeginChild(id,fct,ordre);}

    //! Applique la fonction fonction bool fct(int id) à chaque noeud descendant celui d'identifiant id.
    template<class Fct> bool foreachNode(int id, const Fct & fct, bool ordre = true)
        {return m_managerArbre.foreachNode(id,fct,ordre);}

    //! Renvoie l'arbre de toutes les entités.
    Tree<Ent> getArbre() override
    {
        QList<int> racines = m_managerArbre.getListId(Arbre::Parent,QVariant(QVariant::Int),Arbre::Id,bdd::Condition::Is);
        Tree<Ent> tree;
        for(QList<int>::const_iterator i = racines.cbegin(); i != racines.cend(); ++i)
            tree<<getArbre(*i);
        return tree;
    }

    //! Renvoie l'arbre de racine entity.
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
            throw std::invalid_argument("L'entité transmise en argument de getArbre n'existe pas.");
    }

    //! Renvoie l'arbre l'entity d'identifiant.
    Tree<Ent> getArbre(int id) override
        {return getArbre(Ent(id));}

    //! Renvoie le liste des descendant direct d'entity.
    ListPtr<Ent> getListChilds(const Ent & entity) override
    {
        QList<int> nodeChilds = m_managerArbre.getListId(Arbre::Parent,entity.id(),Arbre::Num);
        ListPtr<Ent> entChilds;
        for(QList<int>::const_iterator i = nodeChilds.cbegin(); i != nodeChilds.cend(); ++i)
         {
            Ent * entPtr = new Ent(*i);
            get(*entPtr);
            entChilds.append(entPtr);
         }
        return entChilds;
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    QList<int> getListChildsId(int id) override
        {return m_managerArbre.getListId(Arbre::Parent,id,Arbre::Num);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    QList<QPair<int,bool>> getListChildsIdLeaf(int id, bool ordre = true) override
    {
        ListPtr<Arbre> childs = m_managerArbre.getList(Arbre::Parent,id,Arbre::Num,ordre);
        QList<QPair<int,bool>> liste;
        for(ListPtr<Arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
            liste.append(QPair<int,bool>((*i).id(),(*i).feuille()));
        return liste;
    }


    //! Renvoie le vecteur des descendant direct d'entity.
    VectorPtr<Ent> getVectorChilds(const Ent & entity) override
    {
        QList<int> nodeChilds = m_managerArbre.getListId(Arbre::Parent,entity.id(),Arbre::Num);
        VectorPtr<Ent> entChilds(nodeChilds.count());
        typename VectorPtr<Ent>::iterator j = entChilds.begin();
        for(QList<int>::const_iterator i = nodeChilds.cbegin(); i != nodeChilds.cend(); ++i, ++j)
         {
            (*j).setId(*i);
            get(*j);
         }
        return entChilds;
    }

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
        {saveConst(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {saveConst(entity);}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(Ent & entity, const Ent & parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(const Ent & entity, const Ent & parent, int num = 0);

    //! Enregistre l'arbre d'entités.
    void save(Tree<Ent> & tree, bdd::TreeSave n = bdd::TreeSave::ExternalChange) override;

protected:
    /*//! Constructeur.
    ManagerOfArbre(const InfoBdd & infoArbre);*/

    /*//! Insert la nouvelle entité entity dans la base de donnée.
    void add(Ent & entity)
    {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        m_link.setId(entity,m_link.nbrAtt()-1);
        execFinish();
    }*/

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent & entity) override
    {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        m_link.setId(entity,nbrAtt()-1);
        execFinish();
    }

    //! Supprime de la Base de données les noeuds hors de l'arbre.
    void deleteLeafOutOf(TreeItem<Ent> * tree);

    //! Permet de lire réccursivement un arbre.
    void getArbreRec(TreeItem<Ent> * tree);

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds.
    void saveAddLeaf(TreeItem<Ent> * tree);

    //! Enregistre l'entité entity en base de donnée.
    void saveConst(const Ent & entity)
    {
        if(entity.isValid())
        {
            if(entity.isNew())
                throw std::invalid_argument("Les nouvelles entités de type arbre ne peut être sauvé "
                                            "dans la base de données uniquement à travers un arbre.");
            else
                saveValide(entity);
        }
        else
            throw std::invalid_argument(messageErreurs(entity).append("\n Erreur de validité").toStdString());
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    void saveExt(TreeItem<Ent> * tree, int idRoot);

    //! Enregistre l'entité valide entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void saveValide(Ent & entity)
        {saveValideConst(entity);}

    //! Enregistre l'entité valide entity en base de donnée.
    void saveValide(const Ent & entity)
        {saveValideConst(entity);}

    //! Enregistre l'entité valide et d'identifiant non nul entity en base de donnée.
    void saveValideConst(const Ent & entity)
    {
        if(exists(entity))
        {
            if(!sameInBdd(entity))
            {
                if(existsUnique(entity) <= bdd::Meme)
                    modify(entity);
                else
                    throw std::invalid_argument(messageErreursUnique(entity)
                                                .append("\n Erreur d'unicité :  il existe déjà dans la base de donnée "
                                                        "une entité ayant les mêmes valeurs d'attributs unique "
                                                        "que l'entité modifiée.").toStdString());
            }
        }
        else
        {
            if(existsUnique(entity) == bdd::Aucun)
                add(entity);
            else
                throw std::invalid_argument(messageErreursUnique(entity)
                                            .append("\n Erreur d'unicité : il existe déjà dans la base de donnée "
                                                    "une entité ayant les mêmes valeurs d'attributs unique "
                                                    "que cette nouvelle entité.").toStdString());
        }
    }

    //! Sauve un arbre où le changement de structure consite seulement
    //! l'ajout de nouveaux noeuds et des permutations à l'interieur de l'arbre.
    void saveWithoutDelete(TreeItem<Ent> * tree);

    //! Réécrie la chaine de requète m_sqlAdd.
    void writeStringSql();
};

template<class Ent> ManagerOfArbre<Ent>::ManagerOfArbre(const InfoBdd & info, const InfoBdd & infoArbre,
                                                        AbstractUniqueSqlTemp<Ent> * unique)
    : ManagerSqlEnt(info,unique),
      m_managerArbre(infoArbre)
    {writeStringSql();}

/*template<class Ent> ManagerOfArbre<Ent>::ManagerOfArbre(const InfoBdd &infoArbre)
    : m_managerArbre(infoArbre)
    {writeStringSql();}*/

template<class Ent> void ManagerOfArbre<Ent>::deleteLeafOutOf(TreeItem<Ent> * tree)
{
    if(tree->hasChild())
    {
        ListPtr<Arbre> childs = m_managerArbre.getList(Arbre::Parent,tree->data().id(),Arbre::Num);
        for(ListPtr<Arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
        {
            if((*i).num() < 0 || (*i).num() >= tree->childCount())
                del((*i).id());
        }
        for(typename QList<TreeItem<Ent>*>::const_iterator i = tree->childs().cbegin(); i != tree->childs().cend(); ++i)
            deleteLeafOutOf(*i);
    }
    else
    {
        QList<int> childs = m_managerArbre.getListId(Arbre::Parent,tree->data().id());
        for(QList<int>::const_iterator i = childs.cbegin(); i != childs.cend(); ++i)
            del(*i);
    }
}

template<class Ent> void ManagerOfArbre<Ent>::getArbreRec(TreeItem<Ent> * tree)
{
    ListPtr<Arbre> childs = m_managerArbre.getList(Arbre::Parent,tree->data().id(),Arbre::Num);
    for(ListPtr<Arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
    {
        TreeItem<Ent> * child = tree->addChild(Ent((*i).id()));
        get(child->modifData());
        if(!(*i).feuille())
            getArbreRec(child);
    }
}

template<class Ent> void ManagerOfArbre<Ent>::save(Ent & entity, const Ent & parent, int num)
{
    if(entity.isValid())
    {
        if(entity.isNew())
        {
            Arbre node(num,parent.id());
            m_managerArbre.save(node);
            entity.setId(node.id());
        }
        else
            m_managerArbre.save(Arbre(num,parent.id(),entity.id()));
        saveValide(entity);
    }
    else
        throw std::invalid_argument(messageErreurs(entity).append("\n Erreur de validité").toStdString());
}

template<class Ent> void ManagerOfArbre<Ent>::save(const Ent & entity, const Ent & parent, int num)
{
    if(entity.isValid())
    {
        if(entity.isNew())
        {
            Arbre node(num,parent.id());
            m_managerArbre.save(node);
            Ent entityTemp(entity);
            entityTemp.setId(node.id());
            saveValide(entityTemp);
        }
        else
        {
            m_managerArbre.save(Arbre(num,parent.id(),entity.id()));
            saveValide(entity);
        }
    }
    else
        throw std::invalid_argument(messageErreurs(entity).append("\n Erreur de validité").toStdString());
}

template<class Ent> void ManagerOfArbre<Ent>::save(Tree<Ent> & tree, bdd::TreeSave n)
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
        {
            if(tree.root()->data().isValid())
            {
                if(tree.root()->data().isNew())
                {
                    Arbre node(0,0);
                    m_managerArbre.save(node);
                    tree.root()->modifData().setId(node.id());
                }
                saveValide(tree.root()->data());
            }
            else
                throw std::invalid_argument(messageErreurs(tree.root()->data()).append("\n Erreur de validité").toStdString());
        }
        switch (n) {
        case TreeSave::AddLeaf:
        case TreeSave::AddLeafWhitoutRoot:
            saveAddLeaf(tree.root());
            break;

        case TreeSave::WithoutDelete:
        case TreeSave::WithoutDeleteWhitoutRoot:
            saveWithoutDelete(tree.root());
            break;

        case TreeSave::InternalChange:
        case TreeSave::InternalChangeWhitoutRoot:
            saveWithoutDelete(tree.root());
            deleteLeafOutOf(tree.root());
            break;

        case TreeSave::ExternalChange:
        case TreeSave::ExternalChangeWhitoutRoot:
            saveExt(tree.root(),tree.root()->data().id());
            deleteLeafOutOf(tree.root());
            break;

        case TreeSave::EntityOnly:
        case TreeSave::EntityOnlyWhitoutRoot:
            break;
        }
    }
}

template<class Ent> void ManagerOfArbre<Ent>::saveAddLeaf(TreeItem<Ent> * tree)
{
    int i = 0;
    for(typename QList<TreeItem<Ent>*>::const_iterator child = tree->childs().cbegin(); child != tree->childs().cend(); ++i, ++child)
    {
        if((*child)->data().isValid())
        {
            if((*child)->data().isNew())
            {
                Arbre node(i,tree->data().id());
                m_managerArbre.save(node);
                (*child)->modifData().setId(node.id());
            }
            saveValide((*child)->data());
            if((*child)->hasChild())
                saveAddLeaf(*child);
        }
        else
            throw std::invalid_argument(messageErreurs((*child)->data()).append("\n Erreur de validité").toStdString());
    }
}

template<class Ent> void ManagerOfArbre<Ent>::saveWithoutDelete(TreeItem<Ent> * tree)
{
    int i = 0;
    int idParent = tree->data().id();
    for(typename QList<TreeItem<Ent>*>::const_iterator child = tree->childs().cbegin(); child != tree->childs().cend(); ++i, ++child)
    {
        if((*child)->data().isValid())
        {
            Arbre node(i,idParent,(*child)->data().id());
            m_managerArbre.saveUnstable(node);
            if((*child)->data().isNew())
                (*child)->modifData().setId(node.id());
            saveValide((*child)->data());
            if((*child)->hasChild())
                saveWithoutDelete(*child);
        }
        else
            throw std::invalid_argument(messageErreurs((*child)->data()).append("\n Erreur de validité").toStdString());
    }
}

template<class Ent> void ManagerOfArbre<Ent>::saveExt(TreeItem<Ent> * tree, int idRoot)
{
    int i = 0;
    int idParent = tree->data().id();
    for(typename QList<TreeItem<Ent>*>::const_iterator child = tree->childs().cbegin(); child != tree->childs().cend(); ++i, ++child)
    {
        if((*child)->data().isValid())
        {
            Arbre node(i,idParent,(*child)->data().id());
            if(node.id() == 0 || m_managerArbre.areRelated(idParent,idRoot))
                m_managerArbre.saveUnstable(node);
            else
                m_managerArbre.saveStUnstable(node);
            if((*child)->data().isNew())
                (*child)->modifData().setId(node.id());
            saveValide((*child)->data());
            if((*child)->hasChild())
                saveExt(*child,idRoot);
        }
        else
            throw std::invalid_argument(messageErreurs((*child)->data()).append("\n Erreur de validité").toStdString());
    }
}

template<class Ent> void ManagerOfArbre<Ent>::writeStringSql()
{
    m_sqlAdd.clear();
    m_sqlAdd.append("INSERT INTO ").append(table()).append("(");
    for(int i = 1; i != nbrAtt(); ++i)
        m_sqlAdd.append(attribut(i)).append(",");
    m_sqlAdd.append(attribut(Ent::Id));
    m_sqlAdd.append(") VALUES(");
    for(int i = 0; i != nbrAtt(); ++i)
        m_sqlAdd.append("?,");
    m_sqlAdd.chop(1);
    m_sqlAdd.append(")");
    m_sqlAdd.squeeze();
}

#endif // MANAGEROFARBRE_H
