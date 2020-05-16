/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/05/2020
 */
#ifndef IDCOMBOBOX_H
#define IDCOMBOBOX_H

#include <QComboBox>
#include "ConteneurPtr.h"
#include "typemps.h"

namespace widgetMPS {
using namespace typeMPS;
/*! \ingroup groupeWidget
 * \brief ComboBox de sélection d'un texte lier àun identifiant.
 */
class IdComboBox : public QComboBox {
    Q_OBJECT
public:
    //! Constructeur.
    using QComboBox::QComboBox;

    //! Renvoie l'identifiant du nom sélectionné.
    idt id() const
        {return currentData().toUInt();}

    //! Mutateur de la liste des labels et identifiants.
    void setText(const std::vector<std::pair<idt,QString>> & noms){
        for(auto i = noms.cbegin(); i != noms.cend(); ++i)
            addItem(i->second,i->first);
    }

    //! Mutateur de la liste des labels et identifiants.
    template<class Ent, class ExtractText> void setText(const conteneurMPS::VectorPtr<Ent> & vec, ExtractText text) {
        for(auto i = vec.cbegin(); i != vec.cend(); ++i)
                addItem(text(*i),i->id());
    }

    //! Mutateur de la liste des labels et identifiants.
    template<class Ent, class ExtractText> void setText(conteneurMPS::VectorPtr<Ent> && vec, ExtractText text) {
        for(auto i = vec.cbegin(); i != vec.cend(); ++i)
                addItem(text(*i),i->id());
    }
};
}
#endif // IDCOMBOBOX_H
