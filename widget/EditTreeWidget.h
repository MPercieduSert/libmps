/*Auteur: PERCIE DU SERT Maxime
 *Date: 12/09/2018
 */
#ifndef EDITTREEWIDGET_H
#define EDITTREEWIDGET_H


#include "ReadTreeWidget.h"
#include "TreeModelEditEntity.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Classe template des widget de type arbre modifiable.
 */
template<class Ent> class EditTreeWidget : ReadTreeWidget<Ent>
{
public:
    using ReadTreeWidget<Ent>::ReadTreeWidget;

    //! Renvoie un pointeur sur le model
    mmps::TreeModelEditEntity<Ent> * model() const
        {return static_cast<mmps::TreeModelEditEntity<Ent> *>(QTreeView::model());}
};
}
#endif // EDITTREEWIDGET_H
