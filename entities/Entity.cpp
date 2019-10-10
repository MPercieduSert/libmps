#include "Entity.h"

QString Entity::affiche() const
        {return QString(name()).append("\n").append(IdAttribut::affiche());}
