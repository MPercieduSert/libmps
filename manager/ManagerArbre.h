/*Auteur: PERCIE DU SERT Maxime
 *Date: 16/09/2016
 */
#ifndef MANAGERARBRE_H
#define MANAGERARBRE_H

#include <cstdlib>
#include "InfoBddBase.h"
#include "ManagerSql.h"
#include "UniqueSqlBase.h"
#include "Arbre.h"

//! \ingroup groupeManager
//! Coprs des deux methodes save pour les arbres.
#define SAVE_ARBRE if(node.isValid()) \
    {if(node.isNew()) add(node); \
    else \
        {if(!sameInBdd(node)) modify(node);}} \
else \
    throw std::invalid_argument(messageErreurs(node).append("\n Erreur de validité").toStdString());

//! \ingroup groupeManager
//! Coprs des deux methodes saveStUnstable pour les arbres.
#define SAVESTUNSTABLE_ARBRE if(node.isValid()) \
    {if(node.isNew()) addUnstable(node); \
    else \
        {if(!sameInBdd(node)) modifyStableUnstable(node);}} \
else \
    throw std::invalid_argument(messageErreurs(node).append("\n Erreur de validité").toStdString());

//! \ingroup groupeManager
//! Coprs des deux methodes saveUnstable pour les arbres.
#define SAVEUNSTABLE_ARBRE if(node.isValid()) \
    {if(node.isNew()) addUnstable(node); \
    else \
        {if(!sameInBdd(node)) modifyUnstable(node);}} \
else \
    throw std::invalid_argument(messageErreurs(node).append("\n Erreur de validité").toStdString());

/*! \ingroup groupeManager
 * \brief Classe des manageurs pour les entités de type arbre.
 */
class ManagerArbre : public ManagerSql<Arbre>
{
protected:
    typedef ManagerSql<Arbre> ManagerSqlArbre;
    QString m_sqlGetParent;     //!< Requéte sql de demande de l'identifiant du parent.

public:
    using ManagerSqlArbre::save;
    using ManagerSqlArbre::sameInBdd;

    //! Constructeur.
    ManagerArbre(const InfoBdd & info);

    //! Destructeur.
    ~ManagerArbre() override = default;

    //! Teste si le noeud d'identifiant idNode est un descendant du noeud d'identifiant idAncestry.
    bool areRelated(idt idNode, idt idAncestry)
    {
        while(idNode != 0 && idNode != idAncestry)
            idNode = getParent(idNode);
        return idNode != 0;
    }

    //! Supprime de la base de donnée le noeud d'identifiant id seulement si c'est une feuille (opération stable).
    bool del(idt id) override;

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui d'identifiant id en
    //! commençant par les descendants.
    template<class Fct> bool foreachBeginChild(idt id, const Fct & fct, bool ordre = true);

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant celui d'identifiant id.
    template<class Fct> bool foreachNode(idt id, const Fct & fct, bool ordre = true);

    //! Returne l'identifiant du parent du noeud d'identifiant id.
    idt getParent(idt id)
    {
        prepare(m_sqlGetParent);
        bindValue(0,id);
        exec();
        if(next())
            return value<idt>();
        else
            return 0;
    }

    //! Return l'indice maximum dans la fratrie du noeud.
    int countBrother(const Arbre & node)
    {return fonctionAgrega(bdd::Agrega::Nombre,Arbre::Parent,Arbre::Parent,node.parent()).toInt();}

    //! Teste s'il y a dans la base de donnée un noeud ayant exactement les mêmes attributs id, num et parent.
    bool sameInBdd(const Arbre & node) override
    {
        Arbre nodeBdd(node.id());
        if(get(nodeBdd))
            return nodeBdd.num() == node.num() && nodeBdd.parent() == node.parent();
        else
            return false;
    }

    //! Réimplémentation des la méthode save.
    void save(Arbre & node) override
        {SAVE_ARBRE}

    //! Réimplémentation des la méthode save.
    void save(const Arbre & node) override
        {SAVE_ARBRE}

    //! Réimplémentation des la méthode save.
    void saveStUnstable(Arbre & node)
        {SAVESTUNSTABLE_ARBRE}

    //! Réimplémentation des la méthode save.
    void saveStUnstable(const Arbre & node)
        {SAVESTUNSTABLE_ARBRE}

    //! Réimplémentation des la méthode save.
    void saveUnstable(Arbre & node)
        {SAVEUNSTABLE_ARBRE}

    //! Réimplémentation des la méthode save.
    void saveUnstable(const Arbre & node)
        {SAVEUNSTABLE_ARBRE}

protected:
    //! Réimplémentation de la méthode add(opération stable).
    void add(Arbre & node) override;

    //! Réimplémentation de la méthode add(opération stable).
    void add(const Arbre & node) override;

    //! Réimplémentation de la méthode add.
    void addUnstable(Arbre & node);

    //! Réimplémentation de la méthode add.
    void addUnstable(const Arbre & node);

    /*//! Supprime de la base de donnée le noeud node ainsi que tous ses déscendants.
    void delUnstable(const Arbre &node);*/

    //! Modifie en base de données le noeud de de couple (parent, num) identique à node en (parent,-num).
    //! Renvoie false si le noeud de couple (parent, num) correspond à node.
    bool deplace(const Arbre & node);

    //! Réimplémentation de la méthode modify.
    void modify(const Arbre & node) override;

    //! Réimplémentation de la méthode modify.
    void modifyUnstable(const Arbre & node);

    //! Réimplémentation de la méthode modify.
    void modifyStableUnstable(const Arbre & node);
};

template<class Fct> bool ManagerArbre::foreachBeginChild(idt id, const Fct &fct, bool ordre)
{
    ListPtr<Arbre> childs = getList(Arbre::Parent,id,Arbre::Num,ordre);
    ListPtr<Arbre>::iterator i = childs.begin();
    while (i != childs.end() && (
               ((*i).feuille() && fct((*i).id()))
               || foreachBeginChild((*i).id(),fct,ordre)
               ))
           ++i;
    return i == childs.end() && fct(id);
}

template<class Fct> bool ManagerArbre::foreachNode(idt id, const Fct &fct, bool ordre)
{
    if(fct(id))
    {
        ListPtr<Arbre> childs = getList(Arbre::Parent,id,Arbre::Num,ordre);
        ListPtr<Arbre>::iterator i = childs.begin();
        while (i != childs.end() (
                    ((*i).feuille() && fct((*i).id()))
                   || foreachNode((*i).id(),fct,ordre)
                    ))
               ++i;
        return i == childs.end();
    }
    return false;
}

#endif // MANAGERARBRE_H
