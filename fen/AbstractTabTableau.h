/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/10/2018
 */
#ifndef ABSTRACTTABTABLEAU_H
#define ABSTRACTTABTABLEAU_H

#include <QStyledItemDelegate>
#include <QTableView>

#include "AbstractTabModule.h"
#include "MAbstractTableModel.h"

/*! \ingroup groupeFen
 * \brief Classe mère des onglets contenant des tableaux
 */
class AbstractTabTableau : public AbstractTabModule
{
    Q_OBJECT
protected:
    QPushButton * m_addColumnButton;        //!< Bouton pour ajouter une colonne
    QStyledItemDelegate * m_delegate;       //!< Déleguer
    MAbstractTableModel * m_model;          //!< Model
    QTableView * m_view;                    //!< Vue
    QPushButton * m_saveButton;             //!< Bouton save
public:
    //! Constructeur.
    explicit AbstractTabTableau(QWidget *parent = nullptr);

    //! Destructeur.
    ~AbstractTabTableau() override = default;

signals:

public slots:
    //! Slot coorepondant à l'action coller.
    void coller() override;
    //! Slot coorepondant à l'action copier.
    void copier() override;
    //! Slot coorepondant à l'action copier.
    void couper() override;
    //! Slot coorepondant à l'action effacer.
    void effacer() override;
    //! Slot coorepondant à l'action sauver.
    void save() override   {m_model->save();}
};

#endif // TABABSTRACTTABLEAU_H
