#include "entity.h"

using namespace mps;
using namespace entities;

DESTR_VIDE_DEF(convert_entity_exception)
DESTR_VIDE_DEF(entity_exception)
DESTR_VIDE_DEF(invalide_entity_exception)

QString entity::affiche() const
        {return QString(name()).append("\n").append(id_attribut::affiche());}

bool entity::hydrate(const QString &attr, const QVariant &value){
    numt j = 0;
    while(j < Nbr_Att && attribut_name(j) != attr)
        ++j;
    if(j != Nbr_Att) {
        set_data_v(j, value);
        return true;
    }
    else
        return false;
}

bool entity::hydrate(std::vector<std::pair<QString,QVariant>> &values){
    auto iter = values.cbegin();
    while (iter != values.cend() && hydrate(iter->first,iter->second))
        ++iter;
    return iter == values.cend();
}

numt entity::position(const QString &attr) const {
    numt pos = 0;
    while(pos < nbr_att() && attribut_name(pos) != attr)
        ++pos;
    return pos;
}
