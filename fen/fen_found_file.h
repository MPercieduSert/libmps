/*Auteur: PERCIE DU SERT Maxime
 *Date: 13/09/2018
 */
#ifndef FEN_FOUND_FILE_H
#define FEN_FOUND_FILE_H

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <stdexcept>
#include "config.h"
#include "file_interface.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 * \brief Cette classe est chargée de la vérifiaction des chemins des fichiers et de leur création s'il sont introuvables.
 */
class fen_found_file : QObject {
protected:
    QWidget *m_modal;
    fichier::file_interface &m_file; //!< Pointeur sur le gestionnaire du fichier que l'on souhaite ouvrrir.

public:
    //! Constructeur. Donner en argument un pointeur sur le gestonnaire de fichier que l'on souhaite ouvrir.
    fen_found_file(fichier::file_interface &file, QWidget *modal = nullptr, QObject *parent = nullptr)
        :QObject(parent), m_modal(modal), m_file(file)
    {}

    //! Destructeur par default.
    ~fen_found_file() override;

    //! Test si le fichier existe, si oui le chemin du fichier est renvoyé sinon une fenêtre de dialogue est ouverte.
    //! Par défaut une copie du fichier est effectuée, mettre copy=false pour ne pas copier le fichier.
    QString exists(bool copy = true);

    //! Test si le fichier est valide, si oui le chemin du fichier est renvoyé sinon une fenêtre de dialogue est ouverte.
    //! Par défaut une copie du fichier est effectuée, mettre copy=false pour ne pas copier le fichier.
    QString is_valid(bool copy = true);

    //! Ouvre un fichier à partir d'un chemin XML dans le fichier de configuration.
    void open_in_conf(fichier::config &conf, const QString &path, bool save = true);

protected:
    /*!
     *\brief Affiche une première fenêtre de choix binaire puis une seconde d'ouverture de fichier.
     *Cette séquence est répétée jusqu'à obtenir un choix valide.
     *
     *Affiche une première fenêtre de choix binaire:
     * + si le bouton de texte accept est cliqué, une fenêtre d'ouverture de fichier est ouverte.
     * + si le bouton de texte reject est cliqué, une QString vide est renvoyée.
     *
     *Cette séquence est répétée de manière récursive jusqu'à obtenir un choix valide.
     *\param text Texte de la première fenêtre
     *\param name Chemin du fichier
     *\param extension Extension du fichier
     *\param accept Texte du boutton accepte
     *\param reject Texte du boutton rejecte
     *\param open_text Texte de la seconde fenêtre d'ouverture de fichier
     *\return Chemin du nouveau fichier
     */
    QString found_file_message(const QString &text, const QString &name, const QString &extension,
                             const QString &accept = "Créer", const QString &reject = "Trouver",
                             const QString &open_text = "Indiquez le nom d'un fichier");

    //! Affiche une QMessageBox modale de texte text avec des bouttons de texte accept et de texte reject.
    //! Retourne true si le bouton de texte accept est cliqué et false sinon.
    bool message(const QString &text, const QString &accept, const QString &reject);
};
}}
#endif // FEN_FOUND_FILE_H
