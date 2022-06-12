/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/12/2019
 */
#ifndef PREDEF_FEN_PRINCIPALE_H
#define PREDEF_FEN_PRINCIPALE_H

#include "dialog_predef.h"
#include <QFileDialog>
#include "predef_zone_centrale.h"
#include "fen_principale.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Fenêtre principale de l'application avec de fonctionnalité prédéfinie.
 *
 * Fenêtre principale:
 * + Crée le menu de gestion de la base de donées.
 *
 */
class predef_fen_principale : public fen_principale {
    Q_OBJECT
protected:
    // Menu
    QMenu *m_bdd_menu;       //!< Menu de gestion de la base de données.
    QMenu *m_bdd_table_menu;  //!< Menu des tables.
    QMenu *m_bdd_xml_menu;    //!< Menu d'importation d'entité de la base de donnée au format xml.

public:
    //! Constructeur.
    explicit predef_fen_principale(abstract_noyau *noyau, std::unique_ptr<b2d::bdd> &&bdd, predef_zone_centrale *central_zone,
                           const QString &bdd_path_xml,
                           const QString &config_path = QDir::currentPath().append("/Config.xml"),
                           QWidget *parent = nullptr);

    //! Destructeur.
    ~predef_fen_principale() override = default;

    //! Accesseur du pointeur de la zone centrale.
    predef_zone_centrale *centrale_zone()
        {return static_cast<predef_zone_centrale *>(m_zone_centrale);}

protected:
    //! Crée le menu bdd.
    void add_menu_bdd();

    //! Connecte une QAction à l'ouverture d'un onglet.
    void connect_action_to_open_tab(QAction *action, const abstract_tab::tab_index &index,
                                const std::vector<QVariant> &vec = std::vector<QVariant>());
public slots:
    //! Exporte des entités de la base de données dans un fichier xml.
    void export_xml();

    //! Importe des entités dans la base de à partir d'un fichier xml fourni par l'utilisateur.
    void import_xml();

    //! Écrit le schema xml de validation des fichier d'importation de données.
    void schema_xml();
};
}}
#endif // PREDEF_FEN_PRINCIPALE_H
