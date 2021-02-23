/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/04/2019
 */
#ifndef ABSTRACT_TAB_MENU_H
#define ABSTRACT_TAB_MENU_H

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWidget>
#include <stdexcept>
#include "abstract_tab_module.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Fenêtre menu d'une annee
 */
class abstract_tab_menu : public abstract_tab_module {
    Q_OBJECT
protected:
    // Widget 
    QPushButton *m_bdd_bouton;      //!< Bouton ouvrant l'onglet de gestion de la base de donnée.
    QPushButton *m_mot_cle_bouton;  //!< Bouton ouvrant l'onglet de gestion des mots clés.
    QSignalMapper *m_bouton_mapper; //!< Lie les boutons à leur actions.

    // Layout
    QHBoxLayout *m_main_layout;     //!< Calque principal.

public:
    //! Ordre de position dans le layout principal
    enum tab_index_type{Single_Tab = -1,
                        Bdd_Tab_Index,
                        Mot_Cle_Tab_Index,
                        Nbr_Tab_Index};

    //! Constructeur
    explicit abstract_tab_menu(const tab_index &index, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_tab_menu() override = default;
};
}}
#endif // ABSTRACT_TAB_MENU_H
