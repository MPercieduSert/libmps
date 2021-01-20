#include "ManagerForArbre.h"

using namespace managerMPS;

ManagerForArbre::ManagerForArbre(const InfoBdd & info)
    : ManagerSqlArbre(info, std::make_unique<ArbreUniqueSql<Arbre>>()) {
    // Get Parent
    m_sqlGetParent.append("SELECT ");
    m_sqlGetParent.append(attribut(Arbre::Parent));
    m_sqlGetParent.append(" FROM ");
    m_sqlGetParent.append(table()).append(" WHERE ");
    m_sqlGetParent.append(attribut(Arbre::Id));
    m_sqlGetParent.append("=?");
    m_sqlGetParent.squeeze();
}

void ManagerForArbre::add(Arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendant doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max) {
                    node.setNum(max+1);
                    ManagerSqlArbre::add(node);
                }
                else {
                    VectorPtr<Arbre> nodes = getList(Arbre::Parent,node.parent(),
                                                 Arbre::Num,node.num(),
                                                 Arbre::Num,
                                                 bmps::condition::Egal,bmps::condition::SupEgal,
                                                 false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlArbre::modify(*i);
                    }
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerForArbre::add(const Arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    ManagerSqlArbre::add(Arbre(true,max+1,node.parent()));
                else {
                    VectorPtr<Arbre> nodes = getList(Arbre::Num,node.num(),Arbre::Num,bmps::condition::SupEgal,false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlArbre::modify(*i);
                    }
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerForArbre::addUnstable(Arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max) {
                    node.setNum(max+1);
                    ManagerSqlArbre::add(node);
                }
                else {
                    deplace(node);
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerForArbre::addUnstable(const Arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            Arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlArbre::add(node);
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    ManagerSqlArbre::add(Arbre(true,max+1,node.parent()));
                else {
                    deplace(node);
                    ManagerSqlArbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlArbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

bool ManagerForArbre::del(idt id) {
    Arbre node(id);
    if(!get(node))
        return true;
    if(!node.feuille() || !ManagerSqlArbre::del(node.id()))
        return false;
    if(node.parent()) {
        if(exists(Arbre::Parent,node.parent())) {
            VectorPtr<Arbre> nodes = getList(Arbre::Num,node.num(),Arbre::Parent,node.parent(),Arbre::Num,bmps::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->setNum(i->num() - 1);
                ManagerSqlArbre::modify(*i);
            }
        }
        else {
            Arbre parent(node.parent());
            get(parent);
            parent.setFeuille(true);
            ManagerSqlArbre::modify(parent);
        }
    }
    return true;
}

/*void ManagerForArbre::delUnstable(const Arbre & node)
{
    if(!node.feuille())
    {
        ListPtr<Arbre> childs(getList(Arbre::Parent,node.id()));
        for(ListPtr<Arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
            delUnstable(*i);
    }
    ManagerSqlArbre::del(node.id());
}*/

bool ManagerForArbre::deplace(const Arbre & node) {
    Arbre nodeBdd(node.num(),node.parent());
    if(existsUnique(nodeBdd)) {
        if(nodeBdd.id() != node.id()) {
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

void ManagerForArbre::modify(const Arbre & node) {
    Arbre nodeBdd(node.id());
    get(nodeBdd);
    auto idParentBdd = nodeBdd.parent();
    auto numBdd = nodeBdd.num();
    if(node.parent()) {
        if(nodeBdd.parent() == node.parent()) {
            auto max = size(node.parent());
            auto num = node.num();
            if(num < 0 || num > max)
                num = max;
            if(numBdd != num) {
                nodeBdd.setNum(-numBdd);
                ManagerSqlArbre::modify(nodeBdd);
                if(std::abs(numBdd - num) == 1) {
                    Arbre nodeSuivant(num,node.parent());
                    getUnique(nodeSuivant);
                    nodeSuivant.setNum(numBdd);
                    ManagerSqlArbre::modify(nodeSuivant);
                }
                else if(num < numBdd) {
                    VectorPtr<Arbre> nodes = getList(Arbre::Parent,node.parent(),Arbre::Num,num,Arbre::Num,numBdd,
                                                Arbre::Num,bmps::condition::Egal,bmps::condition::SupEgal,bmps::condition::Inf,false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlArbre::modify((*i));
                    }
                }
                else {
                    VectorPtr<Arbre> nodes = getList(Arbre::Parent,node.parent(),Arbre::Num,numBdd,Arbre::Num,num,
                                                Arbre::Num,bmps::condition::Egal,bmps::condition::Sup,bmps::condition::InfEgal);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() - 1);
                        ManagerSqlArbre::modify((*i));
                    }
                }
                nodeBdd.setNum(num);
            }
        }
        else {
            nodeBdd.setParent(node.parent());
            Arbre parent(nodeBdd.parent());
            get(parent);
            if(parent.feuille()) {
                parent.setFeuille(false);
                ManagerSqlArbre::modify(parent);
                nodeBdd.setNum(0);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    nodeBdd.setNum(max + 1);
                else {
                    VectorPtr<Arbre> nodes(getList(Arbre::Parent,node.parent(),Arbre::Num,node.num(),
                                                Arbre::Num,bmps::condition::Egal,bmps::condition::SupEgal,false));
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlArbre::modify((*i));
                    }
                    nodeBdd.setNum(node.num());
                }
            }
        }
        ManagerSqlArbre::modify(nodeBdd);
    }
    else {
        nodeBdd.setParent(0);
        nodeBdd.setNum(0);
        ManagerSqlArbre::modify(nodeBdd);
    }

    if(idParentBdd != 0 && idParentBdd != nodeBdd.parent()) {
        if(exists(Arbre::Parent,idParentBdd)) {
            VectorPtr<Arbre> nodes
                    = getList(Arbre::Parent,idParentBdd,Arbre::Num,numBdd,Arbre::Num,bmps::condition::Egal,bmps::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->setNum(i->num() - 1);
                ManagerSqlArbre::modify(*i);
            }
        }
        else {
            Arbre parent(idParentBdd);
            get(parent);
            parent.setFeuille(true);
            ManagerSqlArbre::modify(parent);
        }
    }
}

void ManagerForArbre::modifyUnstable(const Arbre & node) {
    Arbre nodeBdd(node.id());
    get(nodeBdd);
    auto max = size(node.parent());
    if(node.num() < 0 || node.num() > max)
        ManagerSqlArbre::modify(Arbre(nodeBdd.feuille(),max+1,node.parent(),node.id()));
    else {
        if(deplace(node))
            ManagerSqlArbre::modify(node);
    }

    if(node.parent() != nodeBdd.parent()) {
        if(!exists(Arbre::Parent,nodeBdd.parent())) {
            Arbre nodeBddParent(nodeBdd.parent());
            get(nodeBddParent);
            nodeBddParent.setFeuille(true);
            ManagerSqlArbre::modify(nodeBddParent);
        }

        Arbre nodeParent(node.parent());
        get(nodeParent);
        if(nodeParent.feuille()) {
            nodeParent.setFeuille(false);
            ManagerSqlArbre::modify(nodeParent);
        }
    }
}

void ManagerForArbre::modifyStableUnstable(const Arbre & node) {
    Arbre nodeBdd(node.id());
    get(nodeBdd);
    auto max = size(node.parent());
    if(node.num() < 0 || node.num() > max)
        ManagerSqlArbre::modify(Arbre(nodeBdd.feuille(),max+1,node.parent(),node.id()));
    else {
        if(deplace(node))
            ManagerSqlArbre::modify(node);
    }

    if(nodeBdd.parent() != 0 && nodeBdd.parent() != node.parent()) {
        if(exists(Arbre::Parent,nodeBdd.parent())) {
            VectorPtr<Arbre> nodes
                    = getList(Arbre::Parent,nodeBdd.parent(),Arbre::Num,nodeBdd.num(),Arbre::Num,bmps::condition::Egal,bmps::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->setNum(i->num() - 1);
                ManagerSqlArbre::modify((*i));
            }
        }
        else {
            Arbre parent(nodeBdd.parent());
            get(parent);
            parent.setFeuille(true);
            ManagerSqlArbre::modify(parent);
        }
    }
}
