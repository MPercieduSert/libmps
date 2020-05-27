/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/10/2018
 */
#ifndef ABSTRACTTABTABLEAU_H
#define ABSTRACTTABTABLEAU_H

#include <QStyledItemDelegate>
#include <QTableView>

#include "AbstractTabModuleWithBdd.h"
#include "TableModel.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Classe mère des onglets contenant des tableaux
 */
class AbstractTabTableau : public AbstractTabModuleWithBdd {
    Q_OBJECT
protected:
    QPushButton * m_addColumnButton;        //!< Bouton pour ajouter une colonne
    QStyledItemDelegate * m_delegate;       //!< Déleguer
    modelMPS::TableModel * m_model;          //!< Model
    QTableView * m_view;                    //!< Vue
    QPushButton * m_saveButton;             //!< Bouton save
public:
    //! Constructeur.
    explicit AbstractTabTableau(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget *parent = nullptr);

    //! Destructeur.
    ~AbstractTabTableau() override = default;

    //! Action à effectuer lorsque l'onglet devient actif.
    void becomeCurrent() override;

    //! Renvoie un pointeur sur le model.
    virtual modelMPS::TableModel * model() const
        {return m_model;}

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
    void sauver() override   {m_model->save();}
    //! Slot coorepondant à l'action supprimer.
    void supprimer() override;
};
}
#endif // TABABSTRACTTABLEAU_H
