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
    IdComboBox(QWidget * parent = nullptr) : QComboBox(parent)
        {connect(this,qOverload<int>(&IdComboBox::currentIndexChanged),this,[this](){emit idChanged(id());});}

    //! Renvoie l'identifiant du nom sélectionné.
    idt id() const
        {return currentData().toUInt();}

    //! Mutateur de la liste des labels et identifiants.
    void addText(const std::vector<std::pair<idt,QString>> & noms){
        for(auto i = noms.cbegin(); i != noms.cend(); ++i)
            addItem(i->second,i->first);
    }

    //! Mutateur de la liste des labels et identifiants.
    template<class Ent, class ExtractText> void addText(const conteneurMPS::VectorPtr<Ent> & vec, ExtractText text) {
        for(auto i = vec.cbegin(); i != vec.cend(); ++i)
                addItem(text(*i),i->id());
    }

    //! Mutateur de la liste des labels et identifiants.
    template<class Ent, class ExtractText> void addText(conteneurMPS::VectorPtr<Ent> && vec, ExtractText text) {
        for(auto i = vec.cbegin(); i != vec.cend(); ++i)
                addItem(text(*i),i->id());
    }
public slots:
    //! Mutateur de l'élément courant par choix de l'identifiant.
    void setCurrentIndexId(szt id);

signals:
    //! La valeur de l'identifiant change.
    void idChanged(szt id);
};
}
#endif // IDCOMBOBOX_H
