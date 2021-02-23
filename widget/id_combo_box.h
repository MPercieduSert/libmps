/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/05/2020
 */
#ifndef ID_COMBO_BOX_H
#define ID_COMBO_BOX_H

#include <QComboBox>
#include "conteneur_ptr.h"
#include "type_mps.h"

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief ComboBox de sélection d'un texte lier àun identifiant.
 */
class id_combo_box : public QComboBox {
    Q_OBJECT
public:
    //! Constructeur.
    id_combo_box(QWidget *parent = nullptr) : QComboBox(parent)
        {connect(this,qOverload<int>(&id_combo_box::currentIndexChanged),this,[this](){emit id_changed(id());});}

    //! Renvoie l'identifiant du nom sélectionné.
    idt id() const
        {return currentData().toUInt();}

    //! Mutateur de la liste des labels et identifiants.
    void add_text(const std::vector<std::pair<idt,QString>> &noms){
        for(auto i = noms.cbegin(); i != noms.cend(); ++i)
            addItem(i->second,i->first);
    }

    //! Mutateur de la liste des labels et identifiants.
    template<class Ent, class ExtractText> void add_text(const vector_ptr<Ent> &vec, ExtractText text) {
        for(auto it = vec.cbegin(); it != vec.cend(); ++it)
                addItem(text(*it),it->id());
    }

    //! Mutateur de la liste des labels et identifiants.
    template<class Ent, class ExtractText> void add_text(vector_ptr<Ent> &&vec, ExtractText text) {
        for(auto it = vec.cbegin(); it != vec.cend(); ++it)
                addItem(text(*it),it->id());
    }
public slots:
    //! Mutateur de l'élément courant par choix de l'identifiant.
    void set_current_index_id(idt id);

signals:
    //! La valeur de l'identifiant change.
    void id_changed(idt id);
};
}}
#endif // ID_COMBO_BOX_H
