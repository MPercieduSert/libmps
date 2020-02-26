/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGERARBRE_H
#define MANAGERARBRE_H

#include "AbstractManagerArbre.h"
#include "ManagerForArbre.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbre.
 */
template<class Ent> class ManagerArbre : public AbstractManagerArbre<Ent> {
protected:
    ManagerForArbre m_managerArbre;        //!< Manager de la structure d'arbre.

    using Arbre = ebmps::Arbre;
    using ManagerSqlEnt = ManagerSql<Ent>;

    using ManagerSqlEnt::m_link;
    using ManagerSqlEnt::m_sqlAdd;
    using ManagerSqlEnt::execFinish;

    using ManagerSqlEnt::prepare;
    using ManagerSqlEnt::messageErreurs;
    using ManagerSqlEnt::modify;
    using ManagerSqlEnt::saveUnique;
    using ManagerSqlEnt::table;

public:
    using ManagerSqlEnt::del;
    using ManagerSqlEnt::attribut;
    using ManagerSqlEnt::existsUnique;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::get;
    using ManagerSqlEnt::getUnique;
    using ManagerSqlEnt::name;
    using ManagerSqlEnt::nbrAtt;
    using ManagerSqlEnt::sameInBdd;
    using ManagerSqlEnt::save;

    //! Constructeur.
    ManagerArbre(const InfoBdd & info, const InfoBdd & infoArbre,
                 std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique = std::make_unique<NoUniqueSql<Ent>>());

    //! Destructeur.
    ~ManagerArbre() override = default;

    //! Creer la table.
    void creer() override {
        m_managerArbre.creer();
        ManagerSqlEnt::creer();
    }

    //! Supprime de la table en base de donnée l'entité d'identifiant id seulement si c'est une feuille.
    bool del(idt id) override {
        Arbre node(id);
        if(m_managerArbre.get(node)) {
            if(node.feuille() && ManagerSqlEnt::del(id))
                return  m_managerArbre.del(id);
            else
                return false;
        }
        else
            return ManagerSqlEnt::del(id);
    }

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui
    //! d'identifiant id en commençant par les descendants.
    template<class Fct> bool foreachBeginChild(idt id, const Fct & fct, bool ordre = true)
        {return m_managerArbre.foreachBeginLeaf(id,fct,ordre);}

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant celui d'identifiant id.
    template<class Fct> bool foreachNode(idt id, const Fct & fct, bool ordre = true)
        {return m_managerArbre.foreachNode(id,fct,ordre);}

    //! Renvoie le liste des descendant direct d'entity.
    VectorPtr<Ent> getListChilds(const Ent & entity) override {
        auto nodeChilds = getListChildsId(entity.id());
        VectorPtr<Ent> entChilds(nodeChilds.size());
        auto j = entChilds.begin();
        for(auto i = nodeChilds.cbegin(); i != nodeChilds.cend(); ++i) {
            j->setId(*i);
            get(*j);
         }
        return entChilds;
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    std::list<idt> getListChildsId(idt id) override
        {return m_managerArbre.getListId(Arbre::Parent,id,Arbre::Num);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    std::vector<std::pair<idt,bool>> getListChildsIdLeaf(idt id) override
        {return m_managerArbre.getList(Arbre::Parent,id,Arbre::Num)
                .vectorOf([](const Arbre & node){return std::pair<idt,bool>(node.id(),node.feuille());});}

    //! Renvoie la liste des identifiants des racines.
    std::list<idt> getListRacinesId() override
        {return m_managerArbre.getListId(Arbre::Parent,QVariant(QVariant::Int),Arbre::Id,bmps::condition::Is);}

//        VectorPtr<Ent> childs = m_managerArbre.getList(Arbre::Parent,id,Arbre::Num,ordre);
//        std::vector<std::pair<idt,bool>> liste(childs.size());
//        auto j = liste.begin();
//        for(auto i = childs.begin(); i != childs.end(); ++i){
//            j->first = i->id();
//            j->second = i->feuille();
//        }
//        return liste;
//    }

    //! Renvoie les informations de la table arbre associée au manager.
    virtual InfoBdd infoArbre() const
        {return m_managerArbre.info();}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
        {saveConst(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {saveConst(entity);}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(Ent & entity, const Ent & parent, int num = 0) override;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(const Ent & entity, const Ent & parent, int num = 0) override;

    //! Retourne le type du manager.
    virtual unsigned typeManager() const override
        {return bmps::ArbreTypeManager;}

protected:
    //! Constructeur.
    ManagerArbre(const InfoBdd & infoArbre);

    /*//! Insert la nouvelle entité entity dans la base de donnée.
    void add(Ent & entity)
    {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        m_link.setId(entity,m_link.nbrAtt()-1);
        execFinish();
    }*/

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent & entity) override {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        m_link.setId(entity,nbrAtt()-1);
        execFinish();
    }

    //! Teste si le noeud d'identifiant id est une feuille dans la base de donnée en vue d'optimisation.
    bool optiLeaf(idt id) override {
        Arbre node(id);
        return !get(node) || node.feuille();
    }

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds.
    void saveAddLeaf(typename tree<Ent>::iterator iter);

    //! Enregistre l'entité entity en base de donnée.
    void saveConst(const Ent & entity) {
        if(entity.isValid()) {
            if(entity.isNew())
                throw std::invalid_argument("Les nouvelles entités de type arbre ne peut être sauvé "
                                            "dans la base de données uniquement à travers un arbre.");
            else
                saveValide(entity);
        }
        else
            throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::saveConst(const ")
                                                     .append(name()).append("&)"),entity);
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    void saveExt(typename tree<Ent>::iterator iter, idt idRoot);

    //! Sauve la racine de l'arbre (donnée et structure).
    void saveRoot(tree<Ent> & tree) override;

    //! Enregistre l'entité valide entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void saveValide(Ent & entity)
        {saveValideConst(entity);}

    //! Enregistre l'entité valide entity en base de donnée.
    void saveValide(const Ent & entity)
        {saveValideConst(entity);}

    //! Enregistre l'entité valide et d'identifiant non nul entity en base de donnée.
    void saveValideConst(const Ent & entity) {
        auto controle = bmps::None;
        if(exists(entity)) {
            if(!sameInBdd(entity)) {
                controle = existsUnique(entity);
                if( controle <= bmps::Meme) {
                    modify(entity);
                    controle = bmps::None;
                }
            }
        }
        else {
            if(existsUnique(entity) == bmps::Aucun) {
                add(entity);
                controle = bmps::None;
            }
        }
        if(controle != bmps::None){
            if(controle == bmps::Autre) {
                Ent entBdd(entity);
                entBdd.setId(0);
                getUnique(entBdd);
                throw UniqueEntityException(QString("void ManagerArbre<").append(name()).append(">::saveValideConst(const")
                                                                            .append(name()).append("&):\n"),entity,entBdd);
            }
            else
                throw UniqueEntityException(QString("void ManagerArbre<").append(name()).append(">::saveValideConst(const")
                                                                            .append(name()).append("&):\n"),entity);
        }
    }

    //! Sauve un arbre où le changement de structure consite seulement
    //! l'ajout de nouveaux noeuds et des permutations à l'interieur de l'arbre.
    void saveWithoutDelete(typename tree<Ent>::iterator iter);

    //! Réécrie la chaine de requète m_sqlAdd.
    void writeStringSql();
};

template<class Ent> ManagerArbre<Ent>::ManagerArbre(const InfoBdd & info, const InfoBdd & infoArbre,
                                                        std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique)
    : ManagerSqlEnt(info,std::move(unique)),
      m_managerArbre(infoArbre)
    {writeStringSql();}

template<class Ent> ManagerArbre<Ent>::ManagerArbre(const InfoBdd &infoArbre)
    : m_managerArbre(infoArbre)
    {writeStringSql();}



template<class Ent> void ManagerArbre<Ent>::save(Ent & entity, const Ent & parent, int num) {
    if(entity.isValid()) {
        if(entity.isNew()) {
            Arbre node(num,parent.id());
            m_managerArbre.save(node);
            entity.setId(node.id());
        }
        else
            m_managerArbre.save(Arbre(num,parent.id(),entity.id()));
        saveValide(entity);
    }
    else
        throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::save(")
                                                 .append(name()).append("&,const ").append(name())
                                                 .append(" &,int)"),entity);
}

template<class Ent> void ManagerArbre<Ent>::save(const Ent & entity, const Ent & parent, int num) {
    if(entity.isValid()) {
        if(entity.isNew()) {
            Arbre node(num,parent.id());
            m_managerArbre.save(node);
            Ent entityTemp(entity);
            entityTemp.setId(node.id());
            saveValide(entityTemp);
        }
        else {
            m_managerArbre.save(Arbre(num,parent.id(),entity.id()));
            saveValide(entity);
        }
    }
    else
        throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::save(const ")
                                                 .append(name()).append("&,const ").append(name())
                                                 .append(" &,int)"),entity);
}

template<class Ent> void ManagerArbre<Ent>::saveAddLeaf(typename tree<Ent>::iterator iter) {
    auto i = 0;
    for(auto child = iter.beginChild(); iter; ++i, ++child) {
        if(child->isValid()) {
            if(child->isNew()) {
                Arbre node(i,iter->id());
                m_managerArbre.save(node);
                child->setId(node.id());
            }
            saveValide(*child);
            if(!child.leaf())
                saveAddLeaf(child);
        }
        else
            throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::saveAddLeaf(tree<")
                                                     .append(name()).append(">::iterator)"),*child);
    }
}

template<class Ent> void ManagerArbre<Ent>::saveRoot(tree<Ent> & tree) {
    auto iter = tree.begin();
    if(iter->isValid()) {
        if(iter->isNew()) {
            Arbre node(0,0);
            m_managerArbre.save(node);
            iter->setId(node.id());
        }
        saveValide(*iter);
    }
    else
        throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::saveRoot(tree<")
                                                 .append(name()).append("> &)"),*iter);
}

template<class Ent> void ManagerArbre<Ent>::saveWithoutDelete(typename tree<Ent>::iterator iter) {
    auto i = 0;
    for(auto child = iter.beginChild(); child; ++i, ++child) {
        if(child->isValid()) {
            Arbre node(i,iter->id(),child->id());
            m_managerArbre.saveUnstable(node);
            if(child->isNew())
                child->setId(node.id());
            saveValide(*child);
            if(!child.leaf())
                saveWithoutDelete(child);
        }
        else
            throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::saveWithoutDelete(tree<")
                                                     .append(name()).append(">::iterator)"),*child);
    }
}

template<class Ent> void ManagerArbre<Ent>::saveExt(typename tree<Ent>::iterator iter, idt idRoot) {
    auto i = 0;
    for(auto child = iter.beginChild(); child; ++i, ++child) {
        if(child->isValid()) {
            Arbre node(i,iter->id(),child->id());
            if(node.id() == 0 || m_managerArbre.areRelated(iter->id(),idRoot))
                m_managerArbre.saveUnstable(node);
            else
                m_managerArbre.saveStUnstable(node);
            if(child->isNew())
                child->setId(node.id());
            saveValide(*child);
            if(!child.leaf())
                saveExt(child,idRoot);
        }
        else
            throw entityMPS::InvalideEntityException(QString("void ManagerArbre<").append(name()).append(">::saveExt(tree<")
                                                     .append(name()).append(">::iterator,idt)"),*child);
    }
}

template<class Ent> void ManagerArbre<Ent>::writeStringSql()
{
    m_sqlAdd.clear();
    m_sqlAdd.append("INSERT INTO ").append(table()).append("(");
    for(szt i = 1; i != nbrAtt(); ++i)
        m_sqlAdd.append(attribut(i)).append(",");
    m_sqlAdd.append(attribut(Ent::Id));
    m_sqlAdd.append(") VALUES(");
    for(szt i = 0; i != nbrAtt(); ++i)
        m_sqlAdd.append("?,");
    m_sqlAdd.chop(1);
    m_sqlAdd.append(")");
    m_sqlAdd.squeeze();
}
}
#endif // MANAGERARBRE_H
