/*Auteur: PERCIE DU SERT Maxime
 *Date: 17/02/2021
 */
#ifndef DIALOG_PREDEF_H
#define DIALOG_PREDEF_H

#include <bdd.h>
#include <QCheckBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <type_mps.h>
#include <set>

namespace widgetMPS {
using namespace type_mps;
/*! \ingroup groupeWidget
 *\brief Dialogue de selection d'une table.
 */
class SelectTableDialog : public QDialog {
    Q_OBJECT
protected:
    std::set<entidt> m_set;       //!< Ensemble des identifiants séléctionnés.
    // Widget
    QLabel *m_label;                    //!< Question
    QListWidget *m_list_widget;         //!< Sélection des tables
    QCheckBox *m_restriction_cb;        //!< Option d'ajout des restrictions
    QPushButton *m_annuler_bouton;      //!< Bouton d'annulation
    QPushButton *m_valider_bouton;      //!< Bonton de validation

    // Calque
    QHBoxLayout *m_bouton_layout;       //!< Calque des boutons.
    QVBoxLayout *m_main_layout;         //!< Calque principale
public:
    //! Constructeur.
    SelectTableDialog(b2d::Bdd::vector_id_name &&vec, QWidget *parent = nullptr);

    //! Acceseur de l'ensemble des identifiants.
    const std::set<entidt> &set() const
        {return m_set;}

    //! Renvoie un drapeaux sur les options.
    flag option() const;
};
}
#endif // DIALOG_PREDEF_H
