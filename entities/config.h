/*Auteur: PERCIE DU SERT Maxime
 *Date: 06/08/2018
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QString>
#include <stdexcept>
#include "file_interface.h"
#include "xml.h"

namespace mps {
namespace fichier {
/*! \ingroup groupeFile
 *\brief Gestionnaire du fichier de configuration.
 *
 *Gestionnaire du fichier de configuration enregistré sous la forme d'un document XML. Tous les noeuds ont des chemins uniques.
 */
class config : public file_interface {
protected:
    QFile m_file;   //!< Fichier contenant la configuration.

public:
    //! Constructeur. Transmettre en argument le chemin du fichier de configuration.
    config(const QString &file)
        :file_interface(file,"XML files (*.xml)"), m_file(file) {}

    //! Destructeur. Referme le fichier de configuration s'il était ouvert.
    ~config() override;

    /*! \brief Modifie la valeur value du noeud de chemin XML path et les attributs attributes.
     *
     *Modifie la valeur par value du noeud de chemin XML path et modifie les attributs attributes.
     *Si les noeuds du chemin n'existent pas, ils sont créés.
     */
    void add(const QString &path, const QString &value = "",
             const std::map<QString, QString> &attributes = std::map<QString,QString>());

    //! Vérifie si le fichier de chemin name existe et est un fichier de configuration valide, si c'est le cas,
    //! le fichier de configuration est remplacé par une copie du fichier de chemin name.
    bool copy(const QString &name) override;

    //! Créer un fichier de configuration avec les entrées par défault.
    bool creer() override
        {return true;}

    //! Renvoie le chemin du dossier par default.
    virtual QString default_directory()
        {return QDir::homePath();}

    //! Test si le fichier de configuration existe.
    bool exists() const override
        {return m_file.exists();}

    //! Renvoie la valeur du noeud de chemin name.
    QString get_vars(const QString &name);

    //! Renvoie les attributs du noeud de chemin name.
    std::vector<val_atts_t> get_vars_atts(const QString &name);

    //! Test si le fichier de configuration est valide.
    bool is_valid() override;

    //! Modifie la valeur value du noeud de chemin XML path et les attributs attributes. Le noeud doit exister.
    void modify(const QString &path, const QString &value = "",
                const std::map<QString, QString> &attributes = std::map<QString,QString>());

    //! Test si le fichier peut être ouvert en écriture puis le referme.
    bool open() override;

    //! Lit le fichier de configuration et l'enregistre dans doc.
    void read_conf_In(doc_xml &doc);

    //! Modifie le chemin du fichier de configuration.
    void set_name(const QString &name) override;

    //! Test s'il existe un noeud de chemin name.
    bool var_exists(const QString &name);

    //! Recopie l'arbre XML vers le chemin path (utiliser plutôt copy).
    void recopie(const QString &path = QDir::homePath()+"/notecpp/configCopie.xml");

    //! Ecrit l'arbre doc dans le fichier de configuration.
    void write(doc_xml &doc);

protected:
    //! Ajoute ou modifie un attribut renvoye false si le chemin est invalide.
    bool set_attribut(const QString &path, const QString &value = "",
                      const std::map<QString, QString> &attributes = std::map<QString,QString>());

    //! Ouvre le fichier en lecture, lance une exception si le fichier ne s'ouvre pas.
    void open_read() {
        if(!m_file.open(QFile::ReadOnly | QFile::Text))
            throw std::runtime_error("Imposible d'ouvrir pour lecture le fichier :"+m_file.fileName().toStdString());
    }

    //! Ouvre le fichier en écriture, lance une exception si le fichier ne s'ouvre pas.
    void open_write() {
        if(!m_file.open(QFile::WriteOnly | QFile::Text))
            throw std::runtime_error("Imposible d'ouvrir pour ecriture le fichier :"+m_file.fileName().toStdString());
    }

    //! Referme le fichier de configuration.
    void close()
        {m_file.close();}
};
}}
#endif // CONFIG_H
