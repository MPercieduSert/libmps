#include "Entity.h"

using namespace entityMPS;

QString Entity::affiche() const
        {return QString(name()).append("\n").append(IdAttribut::affiche());}
