/*Auteur: PERCIE DU SERT Maxime
 *Date: 22/04/2020
 */
#ifndef TABLEENTITYVIEW_H
#define TABLEENTITYVIEW_H

#include <QTableView>
#include "AbstractEntityTableau.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Vue en tableau des donnée d'un model de type AbstractTableEntityModel.
 */
class TableEntityView : public QTableView {
    Q_OBJECT
protected:

public:
    TableEntityView(QWidget * parent = nullptr);

    ~TableEntityView();
};
}
#endif // TABLEENTITYVIEW_H
