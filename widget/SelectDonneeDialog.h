/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef SELECTDONNEEDIALOG_H
#define SELECTDONNEEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "EntityPredef.h"
#include "ReadTreeWidget.h"

/*! \defgroup groupeDialogue Fenêtre de Dialogue
 * \brief Ensemble de classes représentant des fenêtres de dialogue.
 */

/*! \ingroup groupeDialogue
 * \brief Espace de noms des fenêtres de dialogue.
 */
namespace dialogMPS {

/*! \ingroup groupeDialogue
 * \brief Fenêtre de dialogue de selection d'une donnée.
 */
class SelectDonneeDialog : public QDialog {
    Q_OBJECT
protected:
    using Donnee = entityMPS::Donnee;
    QLabel *m_label;                            //!< Texte d'entête.
    QPushButton *m_cancelBouton;                //!< Bouton d'annulation.
    QPushButton *m_okBouton;                    //!< Bouton de confirmation de selection.
    QHBoxLayout *m_hLayout;                     //!< Calque secondaire.
    QVBoxLayout *m_vLayout;                     //!< Calque principal.
    widgetMPS::ReadTreeWidget<Donnee> * m_treeWidget;      //!< Arbre de données.

public:
    //! Constructeur.
    explicit SelectDonneeDialog(const conteneurMPS::tree<Donnee> & tree, QWidget * parent = nullptr);

    //! Destructeur par defaut.
    ~SelectDonneeDialog() override = default;

public slots:
    //! Surdéfinition.
    void accept() override;

    //! Surdéfinition.
    void reject() override;
};
}
#endif // SELECTDONNEEDIALOG_H
