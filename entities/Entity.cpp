#include "Entity.h"

using namespace entityMPS;

DESTR_VIDE_DEF(ConvertEntityException)
DESTR_VIDE_DEF(EntityException)
DESTR_VIDE_DEF(InvalideEntityException)

QString Entity::affiche() const
        {return QString(name()).append("\n").append(IdAttribut::affiche());}

bool Entity::hydrate(const QString & attr, const QVariant & value){
    numt j = 0;
    while(j < NbrAtt && nameAttribut(j) != attr)
        ++j;
    if(j != NbrAtt) {
        setDataP(j, value);
        return true;
    }
    else
        return false;
}

bool Entity::hydrate(std::vector<std::pair<QString,QVariant>> & values){
    auto iter = values.cbegin();
    while (iter != values.cend() && hydrate(iter->first,iter->second))
        ++iter;
    return iter == values.cend();
}

numt Entity::position(const QString & attr) const {
    numt pos = 0;
    while(pos < nbrAtt() && nameAttribut(pos) != attr)
        ++pos;
    return pos;
}
