#include "ManagerForarbre.h"

using namespace managerMPS;

ManagerForarbre::ManagerForarbre(const InfoBdd &info)
    : ManagerSqlarbre(info, std::make_unique<arbreUniqueSql<arbre>>()) {
    // Get Parent
    m_sqlGetParent.append("SELECT ");
    m_sqlGetParent.append(attribut(arbre::Parent));
    m_sqlGetParent.append(" FROM ");
    m_sqlGetParent.append(table()).append(" WHERE ");
    m_sqlGetParent.append(attribut(arbre::Id));
    m_sqlGetParent.append("=?");
    m_sqlGetParent.squeeze();
}

void ManagerForarbre::add(arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendant doit avoir un numéro nul.");
                else
                    ManagerSqlarbre::add(node);
                parent.setFeuille(false);
                ManagerSqlarbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max) {
                    node.setNum(max+1);
                    ManagerSqlarbre::add(node);
                }
                else {
                    vector_ptr<arbre> nodes = getList(arbre::Parent,node.parent(),
                                                 arbre::Num,node.num(),
                                                 arbre::Num,
                                                 bmps::condition::Egal,bmps::condition::SupEgal,
                                                 false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlarbre::modify(*i);
                    }
                    ManagerSqlarbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlarbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerForarbre::add(const arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlarbre::add(node);
                parent.setFeuille(false);
                ManagerSqlarbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    ManagerSqlarbre::add(arbre(true,max+1,node.parent()));
                else {
                    vector_ptr<arbre> nodes = getList(arbre::Num,node.num(),arbre::Num,bmps::condition::SupEgal,false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlarbre::modify(*i);
                    }
                    ManagerSqlarbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlarbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerForarbre::addUnstable(arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlarbre::add(node);
                parent.setFeuille(false);
                ManagerSqlarbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max) {
                    node.setNum(max+1);
                    ManagerSqlarbre::add(node);
                }
                else {
                    deplace(node);
                    ManagerSqlarbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlarbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void ManagerForarbre::addUnstable(const arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ManagerSqlarbre::add(node);
                parent.setFeuille(false);
                ManagerSqlarbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    ManagerSqlarbre::add(arbre(true,max+1,node.parent()));
                else {
                    deplace(node);
                    ManagerSqlarbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ManagerSqlarbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

bool ManagerForarbre::del(idt id) {
    arbre node(id);
    if(!get(node))
        return true;
    if(!node.feuille() || !ManagerSqlarbre::del(node.id()))
        return false;
    if(node.parent()) {
        if(exists(arbre::Parent,node.parent())) {
            vector_ptr<arbre> nodes = getList(arbre::Num,node.num(),arbre::Parent,node.parent(),arbre::Num,bmps::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->setNum(i->num() - 1);
                ManagerSqlarbre::modify(*i);
            }
        }
        else {
            arbre parent(node.parent());
            get(parent);
            parent.setFeuille(true);
            ManagerSqlarbre::modify(parent);
        }
    }
    return true;
}

/*void ManagerForarbre::delUnstable(const arbre &node)
{
    if(!node.feuille())
    {
        list_ptr<arbre> childs(getList(arbre::Parent,node.id()));
        for(list_ptr<arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
            delUnstable(*i);
    }
    ManagerSqlarbre::del(node.id());
}*/

bool ManagerForarbre::deplace(const arbre &node) {
    arbre nodeBdd(node.num(),node.parent());
    if(existsUnique(nodeBdd)) {
        if(nodeBdd.id() != node.id()) {
            nodeBdd.setNum(-nodeBdd.num());
            ManagerSqlarbre::modify(nodeBdd);
            return true;
        }
        else
            return false;
    }
    else
        return true;
}

void ManagerForarbre::modify(const arbre &node) {
    arbre nodeBdd(node.id());
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
                ManagerSqlarbre::modify(nodeBdd);
                if(std::abs(numBdd - num) == 1) {
                    arbre nodeSuivant(num,node.parent());
                    getUnique(nodeSuivant);
                    nodeSuivant.setNum(numBdd);
                    ManagerSqlarbre::modify(nodeSuivant);
                }
                else if(num < numBdd) {
                    vector_ptr<arbre> nodes = getList(arbre::Parent,node.parent(),arbre::Num,num,arbre::Num,numBdd,
                                                arbre::Num,bmps::condition::Egal,bmps::condition::SupEgal,bmps::condition::Inf,false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlarbre::modify((*i));
                    }
                }
                else {
                    vector_ptr<arbre> nodes = getList(arbre::Parent,node.parent(),arbre::Num,numBdd,arbre::Num,num,
                                                arbre::Num,bmps::condition::Egal,bmps::condition::Sup,bmps::condition::InfEgal);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() - 1);
                        ManagerSqlarbre::modify((*i));
                    }
                }
                nodeBdd.setNum(num);
            }
        }
        else {
            nodeBdd.set_parent(node.parent());
            arbre parent(nodeBdd.parent());
            get(parent);
            if(parent.feuille()) {
                parent.setFeuille(false);
                ManagerSqlarbre::modify(parent);
                nodeBdd.setNum(0);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    nodeBdd.setNum(max + 1);
                else {
                    vector_ptr<arbre> nodes(getList(arbre::Parent,node.parent(),arbre::Num,node.num(),
                                                arbre::Num,bmps::condition::Egal,bmps::condition::SupEgal,false));
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->setNum(i->num() + 1);
                        ManagerSqlarbre::modify((*i));
                    }
                    nodeBdd.setNum(node.num());
                }
            }
        }
        ManagerSqlarbre::modify(nodeBdd);
    }
    else {
        nodeBdd.set_parent(0);
        nodeBdd.setNum(0);
        ManagerSqlarbre::modify(nodeBdd);
    }

    if(idParentBdd != 0 &&idParentBdd != nodeBdd.parent()) {
        if(exists(arbre::Parent,idParentBdd)) {
            vector_ptr<arbre> nodes
                    = getList(arbre::Parent,idParentBdd,arbre::Num,numBdd,arbre::Num,bmps::condition::Egal,bmps::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->setNum(i->num() - 1);
                ManagerSqlarbre::modify(*i);
            }
        }
        else {
            arbre parent(idParentBdd);
            get(parent);
            parent.setFeuille(true);
            ManagerSqlarbre::modify(parent);
        }
    }
}

void ManagerForarbre::modifyUnstable(const arbre &node) {
    arbre nodeBdd(node.id());
    get(nodeBdd);
    auto max = size(node.parent());
    if(node.num() < 0 || node.num() > max)
        ManagerSqlarbre::modify(arbre(nodeBdd.feuille(),max+1,node.parent(),node.id()));
    else {
        if(deplace(node))
            ManagerSqlarbre::modify(node);
    }

    if(node.parent() != nodeBdd.parent()) {
        if(!exists(arbre::Parent,nodeBdd.parent())) {
            arbre nodeBddParent(nodeBdd.parent());
            get(nodeBddParent);
            nodeBddParent.setFeuille(true);
            ManagerSqlarbre::modify(nodeBddParent);
        }

        arbre nodeParent(node.parent());
        get(nodeParent);
        if(nodeParent.feuille()) {
            nodeParent.setFeuille(false);
            ManagerSqlarbre::modify(nodeParent);
        }
    }
}

void ManagerForarbre::modifyStableUnstable(const arbre &node) {
    arbre nodeBdd(node.id());
    get(nodeBdd);
    auto max = size(node.parent());
    if(node.num() < 0 || node.num() > max)
        ManagerSqlarbre::modify(arbre(nodeBdd.feuille(),max+1,node.parent(),node.id()));
    else {
        if(deplace(node))
            ManagerSqlarbre::modify(node);
    }

    if(nodeBdd.parent() != 0 &&nodeBdd.parent() != node.parent()) {
        if(exists(arbre::Parent,nodeBdd.parent())) {
            vector_ptr<arbre> nodes
                    = getList(arbre::Parent,nodeBdd.parent(),arbre::Num,nodeBdd.num(),arbre::Num,bmps::condition::Egal,bmps::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->setNum(i->num() - 1);
                ManagerSqlarbre::modify((*i));
            }
        }
        else {
            arbre parent(nodeBdd.parent());
            get(parent);
            parent.setFeuille(true);
            ManagerSqlarbre::modify(parent);
        }
    }
}
