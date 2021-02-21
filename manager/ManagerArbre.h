/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGERARBRE_H
#define MANAGERARBRE_H

#include "AbstractManagerarbre.h"
#include "ManagerForarbre.h"

namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbre.
 */
template<class Ent> class Managerarbre : public AbstractManagerarbre<Ent> {
protected:
    ManagerForarbre m_managerarbre;         //!< Manager de la structure d'arbre.

    using arbre = ebmps::arbre;
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
    using ManagerSqlEnt::nbr_att;
    using ManagerSqlEnt::sameInBdd;
    using ManagerSqlEnt::save;

    //! Constructeur.
    Managerarbre(const InfoBdd &info, const InfoBdd &infoarbre,
                 std::unique_ptr<AbstractUniqueSql_temp<Ent>> &&unique = std::make_unique<NoUniqueSql<Ent>>());

    //! Destructeur.
    ~Managerarbre() override = default;

    //! Creer la table.
    void creer() override {
        m_managerarbre.creer();
        ManagerSqlEnt::creer();
    }

    //! Supprime de la table en base de donnée l'entité d'identifiant id seulement si c'est une feuille.
    bool del(idt id) override {
        arbre node(id);
        if(m_managerarbre.get(node)) {
            if(node.feuille() &&ManagerSqlEnt::del(id))
                return  m_managerarbre.del(id);
            else
                return false;
        }
        else
            return ManagerSqlEnt::del(id);
    }

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui
    //! d'identifiant id en commençant par les descendants.
    template<class Fct> bool foreachBeginChild(idt id, const Fct &fct, bool ordre = true)
        {return m_managerarbre.foreachBeginLeaf(id,fct,ordre);}

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant celui d'identifiant id.
    template<class Fct> bool foreachNode(idt id, const Fct &fct, bool ordre = true)
        {return m_managerarbre.foreachNode(id,fct,ordre);}

    //! Renvoie l'identifiant du parent (si le manager est de type arbre).
    idt getIdParent(idt id) override
        {return m_managerarbre.getParent(id);}

    //! Renvoie le liste des descendant direct d'entity.
    vector_ptr<Ent> getListChilds(const Ent &ent) override {
        auto nodeChilds = getListChildsId(entity.id());
        vector_ptr<Ent> entChilds(nodeChilds.size());
        auto j = entChilds.begin();
        for(auto i = nodeChilds.cbegin(); i != nodeChilds.cend(); ++i) {
            j->set_id(*i);
            get(*j);
         }
        return entChilds;
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    std::list<idt> getListChildsId(idt id) override
        {return m_managerarbre.getListId(arbre::Parent,id,arbre::Num);}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    std::vector<std::pair<idt,bool>> getListChildsIdLeaf(idt id) override
        {return m_managerarbre.getList(arbre::Parent,id,arbre::Num)
                .vector_of([](const arbre &node){return std::pair<idt,bool>(node.id(),node.feuille());});}

    //! Renvoie la liste des identifiants des racines.
    std::list<idt> getListRacinesId() override
        {return m_managerarbre.getListId(arbre::Parent,QVariant(QVariant::Int),arbre::Id,bmps::condition::Is);}

//        vector_ptr<Ent> childs = m_managerarbre.getList(arbre::Parent,id,arbre::Num,ordre);
//        std::vector<std::pair<idt,bool>> liste(childs.size());
//        auto j = liste.begin();
//        for(auto i = childs.begin(); i != childs.end(); ++i){
//            j->first = i->id();
//            j->second = i->feuille();
//        }
//        return liste;
//    }

    //! Renvoie les informations de la table arbre associée au manager.
    virtual InfoBdd infoarbre() const
        {return m_managerarbre.info();}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(Ent &ent, idt idParent, int num = 0) override;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const Ent &ent, idt idParent, int num = 0) override;

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override
        {saveConst(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {saveConst(entity);}

    //! Renvoie le nombre de fils du noeud.
    int size_child(const entity &ent) override
        {return m_managerarbre.size(entity.id());}

    //! Retourne le type du manager.
    virtual flag typeManager() const override
        {return bmps::arbreTypeManager;}

protected:
    //! Constructeur.
    Managerarbre(const InfoBdd &infoarbre);

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent &ent) override {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        m_link.set_id(entity,nbr_att()-1);
        execFinish();
    }

    //! Teste si le noeud d'identifiant id est une feuille dans la base de donnée en vue d'optimisation.
    bool optiLeaf(idt id) override {
        arbre node(id);
        return !get(node) || node.feuille();
    }

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds.
    void saveAddLeaf(typename tree<Ent>::iterator iter) override;

    //! Enregistre l'entité entity en base de donnée.
    void saveConst(const Ent &ent) {
        if(entity.is_valid()) {
            if(entity.is_new())
                throw std::invalid_argument("Les nouvelles entités de type arbre ne peut être sauvé "
                                            "dans la base de données uniquement à travers un arbre.");
            else
                save_valide(entity);
        }
        else
            throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::saveConst(const ")
                                                     .append(name()).append("&)"),entity);
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    void saveExt(typename tree<Ent>::iterator iter, idt idRoot) override;

    //! Sauve la racine de l'arbre (donnée et structure).
    void saveRoot(tree<Ent> &tree) override;

    //! Enregistre l'entité valide et d'identifiant non nul entity en base de donnée.
    void save_valide(const Ent &ent) {
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
                entBdd.set_id(0);
                getUnique(entBdd);
                throw Uniqueentity_exception(QString("void Managerarbre<").append(name()).append(">::save_valideConst(const")
                                                                            .append(name()).append("&):\n"),entity,entBdd);
            }
            else
                throw Uniqueentity_exception(QString("void Managerarbre<").append(name()).append(">::save_valideConst(const")
                                                                            .append(name()).append("&):\n"),entity);
        }
    }

    //! Sauve un arbre où le changement de structure consite seulement
    //! l'ajout de nouveaux noeuds et des permutations à l'interieur de l'arbre.
    void saveWithoutDelete(typename tree<Ent>::iterator iter) override;

    //! Réécrie la chaine de requète m_sqlAdd.
    void writeStringSql();
};

template<class Ent> Managerarbre<Ent>::Managerarbre(const InfoBdd &info, const InfoBdd &infoarbre,
                                                        std::unique_ptr<AbstractUniqueSql_temp<Ent>> &&unique)
    : ManagerSqlEnt(info,std::move(unique)),
      m_managerarbre(infoarbre)
    {writeStringSql();}

template<class Ent> Managerarbre<Ent>::Managerarbre(const InfoBdd &infoarbre)
    : m_managerarbre(infoarbre)
    {writeStringSql();}



template<class Ent> void Managerarbre<Ent>::insert(Ent &ent, idt idParent, int num) {
    if(entity.is_valid()) {
        if(entity.is_new()) {
            arbre node(num,idParent);
            m_managerarbre.save(node);
            entity.set_id(node.id());
        }
        else
            m_managerarbre.save(arbre(num,idParent,entity.id()));
        save_valide(entity);
    }
    else
        throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::save(")
                                                 .append(name()).append("&,const ").append(name())
                                                 .append(" &,int)"),entity);
}

template<class Ent> void Managerarbre<Ent>::insert(const Ent &ent, idt idParent, int num) {
    if(entity.is_valid()) {
        if(entity.is_new()) {
            arbre node(num,idParent);
            m_managerarbre.save(node);
            Ent entity_temp(entity);
            entity_temp.set_id(node.id());
            save_valide(entity_temp);
        }
        else {
            m_managerarbre.save(arbre(num,idParent,entity.id()));
            save_valide(entity);
        }
    }
    else
        throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::save(const ")
                                                 .append(name()).append("&,const ").append(name())
                                                 .append(" &,int)"),entity);
}

template<class Ent> void Managerarbre<Ent>::saveAddLeaf(typename tree<Ent>::iterator iter) {
    auto i = 0;
    for(auto child = iter.begin_child(); iter; ++i, ++child) {
        if(child->is_valid()) {
            if(child->is_new()) {
                arbre node(i,iter->id());
                m_managerarbre.save(node);
                child->set_id(node.id());
            }
            save_valide(*child);
            if(!child.leaf())
                saveAddLeaf(child);
        }
        else
            throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::saveAddLeaf(tree<")
                                                     .append(name()).append(">::iterator)"),*child);
    }
}

template<class Ent> void Managerarbre<Ent>::saveRoot(tree<Ent> &tree) {
    auto iter = tree.begin();
    if(iter->is_valid()) {
        if(iter->is_new()) {
            arbre node(0,0);
            m_managerarbre.save(node);
            iter->set_id(node.id());
        }
        save_valide(*iter);
    }
    else
        throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::saveRoot(tree<")
                                                 .append(name()).append("> &)"),*iter);
}

template<class Ent> void Managerarbre<Ent>::saveWithoutDelete(typename tree<Ent>::iterator iter) {
    auto i = 0;
    for(auto child = iter.begin_child(); child; ++i, ++child) {
        if(child->is_valid()) {
            arbre node(i,iter->id(),child->id());
            m_managerarbre.saveUnstable(node);
            if(child->is_new())
                child->set_id(node.id());
            save_valide(*child);
            if(!child.leaf())
                saveWithoutDelete(child);
        }
        else
            throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::saveWithoutDelete(tree<")
                                                     .append(name()).append(">::iterator)"),*child);
    }
}

template<class Ent> void Managerarbre<Ent>::saveExt(typename tree<Ent>::iterator iter, idt idRoot) {
    auto i = 0;
    for(auto child = iter.begin_child(); child; ++i, ++child) {
        if(child->is_valid()) {
            arbre node(i,iter->id(),child->id());
            if(node.id() == 0 || m_managerarbre.areRelated(iter->id(),idRoot))
                m_managerarbre.saveUnstable(node);
            else
                m_managerarbre.saveStUnstable(node);
            if(child->is_new())
                child->set_id(node.id());
            save_valide(*child);
            if(!child.leaf())
                saveExt(child,idRoot);
        }
        else
            throw entityMPS::invalide_entity_exception(QString("void Managerarbre<").append(name()).append(">::saveExt(tree<")
                                                     .append(name()).append(">::iterator,idt)"),*child);
    }
}

template<class Ent> void Managerarbre<Ent>::writeStringSql()
{
    m_sqlAdd.clear();
    m_sqlAdd.append("INSERT INTO ").append(table()).append("(");
    for(post i = 1; i != nbr_att(); ++i)
        m_sqlAdd.append(attribut(i)).append(",");
    m_sqlAdd.append(attribut(Ent::Id));
    m_sqlAdd.append(") VALUES(");
    for(post i = 0; i != nbr_att(); ++i)
        m_sqlAdd.append("?,");
    m_sqlAdd.chop(1);
    m_sqlAdd.append(")");
    m_sqlAdd.squeeze();
}
}
#endif // MANAGERARBRE_H
