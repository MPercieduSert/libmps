#include "ManagerArbre.h"

ManagerArbre::ManagerArbre(const InfoBdd & info)
    : ManagerSqlArbre(info, new ArbreUniqueSql<Arbre>())
{
    // Get Parent
    m_sqlGetParent.append("SELECT ");
    m_sqlGetParent.append(attribut(Arbre::Parent));
    m_sqlGetParent.append(" FROM ");
    m_sqlGetParent.append(table()).append(" WHERE ");
    m_sqlGetParent.append(attribut(Arbre::Id));
    m_sqlGetParent.append("=?");
    m_sqlGetParent.squeeze();
}

void ManagerArbre::add(Arbre &node)
{
    if(node.feuille())
    {
        if(node.parent())
        {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille())
            {
                if(node.num())
                    throw std::invalid_argument("Le premier descendant doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else
            {
                int max = countBrother(node);
                if(node.num() < 0 || node.num() > max)
                {
                    node.setNum(max+1);
                    ManagerSqlArbre::add(node);
                }
                else
                {
                    ListPtr<Arbre> nodes(getList(Arbre::Parent,node.parent(),
                                                 Arbre::Num,node.num(),
                                                 Arbre::Num,
                                                 bdd::Condition::Egal,bdd::Condition::SupEgal,
                                                 false));
                    for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                    {
                        (*i).setNum((*i).num() + 1);
                        ManagerSqlArbre::modify(*i);
                    }
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else
        {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerArbre::add(const Arbre &node)
{
    if(node.feuille())
    {
        if(node.parent())
        {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille())
            {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else
            {
                int max = countBrother(node);
                if(node.num() < 0 || node.num() > max)
                    ManagerSqlArbre::add(Arbre(true,max+1,node.parent()));
                else
                {
                    ListPtr<Arbre> nodes(getList(Arbre::Num,node.num(),Arbre::Num,bdd::Condition::SupEgal,false));
                    for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                    {
                        (*i).setNum((*i).num() + 1);
                        ManagerSqlArbre::modify(*i);
                    }
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else
        {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerArbre::addUnstable(Arbre &node)
{
    if(node.feuille())
    {
        if(node.parent())
        {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille())
            {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else
            {
                int max = countBrother(node);
                if(node.num() < 0 || node.num() > max)
                {
                    node.setNum(max+1);
                    ManagerSqlArbre::add(node);
                }
                else
                {
                    deplace(node);
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else
        {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerArbre::addUnstable(const Arbre &node)
{
    if(node.feuille())
    {
        if(node.parent())
        {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille())
            {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else
            {
                int max = countBrother(node);
                if(node.num() < 0 || node.num() > max)
                    ManagerSqlArbre::add(Arbre(true,max+1,node.parent()));
                else
                {
                    deplace(node);
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else
        {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

bool ManagerArbre::del(int id)
{
    Arbre node(id);
    if(!get(node))
        return true;
    if(!node.feuille() || !ManagerSqlArbre::del(node.id()))
        return false;
    if(node.parent())
    {
        if(exists(Arbre::Parent,node.parent()))
        {
            ListPtr<Arbre> nodes(getList(Arbre::Num,node.num(),Arbre::Parent,node.parent(),Arbre::Num,bdd::Condition::Sup));
            for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i).setNum((*i).num() - 1);
                ManagerSqlArbre::modify(*i);
            }
        }
        else
        {
            Arbre parent(node.parent());
            get(parent);
            parent.setFeuille(true);
            ManagerSqlArbre::modify(parent);
        }
    }
    return true;
}

/*void ManagerArbre::delUnstable(const Arbre & node)
{
    if(!node.feuille())
    {
        ListPtr<Arbre> childs(getList(Arbre::Parent,node.id()));
        for(ListPtr<Arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
            delUnstable(*i);
    }
    ManagerSqlArbre::del(node.id());
}*/

bool ManagerArbre::deplace(const Arbre & node)
{
    Arbre nodeBdd(node.num(),node.parent());
    if(existsUnique(nodeBdd))
    {
        if(nodeBdd.id() != node.id())
        {
            nodeBdd.setNum(-nodeBdd.num());
            ManagerSqlArbre::modify(nodeBdd);
            return true;
        }
        else
            return false;
    }
    else
        return true;
}

void ManagerArbre::modify(const Arbre & node)
{
    Arbre nodeBdd(node.id());
    get(nodeBdd);
    int idParentBdd = nodeBdd.parent();
    int numBdd = nodeBdd.num();
    if(node.parent())
    { 
        if(nodeBdd.parent() == node.parent())
        {
            int max = countBrother(node);
            int num = node.num();
            if(num < 0 || num > max)
                num = max;
            if(numBdd != num)
            {
                nodeBdd.setNum(-numBdd);
                ManagerSqlArbre::modify(nodeBdd);
                if(std::abs(numBdd - num) == 1)
                {
                    Arbre nodeSuivant(num,node.parent());
                    getUnique(nodeSuivant);
                    nodeSuivant.setNum(numBdd);
                    ManagerSqlArbre::modify(nodeSuivant);
                }
                else if(num < numBdd)
                {
                    ListPtr<Arbre> nodes(getList(Arbre::Parent,node.parent(),Arbre::Num,num,Arbre::Num,numBdd,
                                                Arbre::Num,bdd::Condition::Egal,bdd::Condition::SupEgal,bdd::Condition::Inf,false));
                    for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                    {
                        (*i).setNum((*i).num() + 1);
                        ManagerSqlArbre::modify((*i));
                    }
                }
                else
                {
                    ListPtr<Arbre> nodes(getList(Arbre::Parent,node.parent(),Arbre::Num,numBdd,Arbre::Num,num,
                                                Arbre::Num,bdd::Condition::Egal,bdd::Condition::Sup,bdd::Condition::InfEgal));
                    for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                    {
                        (*i).setNum((*i).num() - 1);
                        ManagerSqlArbre::modify((*i));
                    }
                }
                nodeBdd.setNum(num);
            }
        }
        else
        {
            nodeBdd.setParent(node.parent());
            Arbre parent(nodeBdd.parent());
            get(parent);
            if(parent.feuille())
            {
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
                nodeBdd.setNum(0);
            }
            else
            {
                int max = countBrother(node);
                if(node.num() < 0 || node.num() > max)
                    nodeBdd.setNum(max + 1);
                else
                {
                    ListPtr<Arbre> nodes(getList(Arbre::Parent,node.parent(),Arbre::Num,node.num(),
                                                Arbre::Num,bdd::Condition::Egal,bdd::Condition::SupEgal,false));
                    for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                    {
                        (*i).setNum((*i).num() + 1);
                        ManagerSqlArbre::modify((*i));
                    }
                    nodeBdd.setNum(node.num());
                }
            }
        }
        ManagerSqlArbre::modify(nodeBdd);
    }
    else
    {
        nodeBdd.setParent(0);
        nodeBdd.setNum(0);
        ManagerSqlArbre::modify(nodeBdd);
    }

    if(idParentBdd != 0 && idParentBdd != nodeBdd.parent())
    {
        if(exists(Arbre::Parent,idParentBdd))
        {
            ListPtr<Arbre> nodes(getList(Arbre::Parent,idParentBdd,Arbre::Num,numBdd,Arbre::Num,bdd::Condition::Egal,bdd::Condition::Sup));
            for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i).setNum((*i).num() - 1);
                ManagerSqlArbre::modify((*i));
            }
        }
        else
        {
            Arbre parent(idParentBdd);
            get(parent);
            parent.setFeuille(true);
            ManagerSqlArbre::modify(parent);
        }
    }
}

void ManagerArbre::modifyUnstable(const Arbre & node)
{
    Arbre nodeBdd(node.id());
    get(nodeBdd);
    int max = countBrother(node);
    if(node.num() < 0 || node.num() > max)
        ManagerSqlArbre::modify(Arbre(nodeBdd.feuille(),max+1,node.parent(),node.id()));
    else
    {
        if(deplace(node))
            ManagerSqlArbre::modify(node);
    }

    if(node.parent() != nodeBdd.parent())
    {
        if(!exists(Arbre::Parent,nodeBdd.parent()))
        {
            Arbre nodeBddParent(nodeBdd.parent());
            get(nodeBddParent);
            nodeBddParent.setFeuille(true);
            ManagerSqlArbre::modify(nodeBddParent);
        }

        Arbre nodeParent(node.parent());
        get(nodeParent);
        if(nodeParent.feuille())
        {
            nodeParent.setFeuille(false);
            ManagerSqlArbre::modify(nodeParent);
        }
    }
}

void ManagerArbre::modifyStableUnstable(const Arbre & node)
{
    Arbre nodeBdd(node.id());
    get(nodeBdd);
    int max = countBrother(node);
    if(node.num() < 0 || node.num() > max)
        ManagerSqlArbre::modify(Arbre(nodeBdd.feuille(),max+1,node.parent(),node.id()));
    else
    {
        if(deplace(node))
            ManagerSqlArbre::modify(node);
    }

    if(nodeBdd.parent() != 0 && nodeBdd.parent() != node.parent())
    {
        if(exists(Arbre::Parent,nodeBdd.parent()))
        {
            ListPtr<Arbre> nodes(getList(Arbre::Parent,nodeBdd.parent(),Arbre::Num,nodeBdd.num(),Arbre::Num,bdd::Condition::Egal,bdd::Condition::Sup));
            for(ListPtr<Arbre>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i).setNum((*i).num() - 1);
                ManagerSqlArbre::modify((*i));
            }
        }
        else
        {
            Arbre parent(nodeBdd.parent());
            get(parent);
            parent.setFeuille(true);
            ManagerSqlArbre::modify(parent);
        }
    }
}
