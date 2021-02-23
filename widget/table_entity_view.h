/*Auteur: PERCIE DU SERT Maxime
 *Date: 22/04/2020
 */
#ifndef TABLE_ENTITY_VIEW_H
#define TABLE_ENTITY_VIEW_H

#include <QTableView>
#include "abstract_entity_tableau.h"

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Vue en tableau des donnée d'un model de type AbstractTableentityModel.
 */
class table_entity_view : public QTableView {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    table_entity_view(QWidget *parent = nullptr);

    //! Destructeur.
    ~table_entity_view();
};
}}
#endif // TABLE_ENTITY_VIEW_H
