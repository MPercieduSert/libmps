/*Auteur: PERCIE DU SERT Maxime
 *Date: 22/04/2020
 */
#ifndef TABLEENTITYVIEW_H
#define TABLEENTITYVIEW_H

#include <QTableView>
#include "abstract_entity_tableau.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 *\brief Vue en tableau des donnée d'un model de type AbstractTableentityModel.
 */
class TableentityView : public QTableView {
    Q_OBJECT
protected:

public:
    TableentityView(QWidget *parent = nullptr);

    ~TableentityView();
};
}
#endif // TABLEENTITYVIEW_H
