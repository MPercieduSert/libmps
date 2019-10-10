/*Auteur: PERCIE DU SERT Maxime
 *Date: 06/08/2018
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QDir>
#include <QMap>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <stdexcept>
#include "FileInterface.h"
#include "xmlmps.h"

/*! \ingroup groupeFile
 * \brief Gestionnaire du fichier de configuration.
 *
 * Gestionnaire du fichier de configuration enregistré sous la forme d'un document XML. Tous les noeuds ont des chemins uniques.
 */
class Config : public FileInterface
{
protected:
    QFile m_confFile;   //!< Fichier contenant la configuration.

public:
    //! Constructeur. Transmettre en argument le chemin du fichier de configuration.
    Config(const QString & file):FileInterface(file,"XML files (*.xml)"), m_confFile(file)
        {}

    //! Destructeur. Referme le fichier de configuration s'il était ouvert.
    ~Config() override;

    /*! \brief Modifie la valeur value du noeud de chemin XML path et les attributs attributes.
     *
     * Modifie la valeur par value du noeud de chemin XML path et modifie les attributs attributes.
     * Si les noeuds du chemin n'existent pas, ils sont créés.
     */
    void add(const QString &path, const QString &value = "", const QMap<QString, QString> &attributes = QMap<QString,QString>());

    //! Vérifie si le fichier de chemin name existe et est un fichier de configuration valide, si c'est le cas,
    //! le fichier de configuration est remplacé par une copie du fichier de chemin name.
    bool copy(const QString & name) override;

    //! Créer un fichier de configuration avec les entrées par défault.
    bool creer() override
        {return true;}

    //! Renvoie le chemin du dossier par default.
    virtual QString defaultDirectory()
        {return QDir::homePath();}

    //! Test si le fichier de configuration existe.
    bool exists() const override
        {return m_confFile.exists();}

    //! Renvoie la valeur du noeud de chemin name.
    QString getVars(const QString & name);


    //QStringList getVars(const QString & name, const QMap<QString,QString> & atts  = QMap<QString,QString>());

    //! Renvoie les attributs du noeud de chemin name.
    QVector<varAtts> getVarsAtts(const QString & name);

    //QVector<varAtts> getVarsAtts(const QString & name, const QString & attName, const QString & attValue);
    //QVector<varAtts> getVarsAtts(const QString & name, const QMap<QString,QString> & atts = QMap<QString,QString>());

    //! Test si le fichier de configuration est valide.
    bool isValid() override;

    //! Modifie la valeur value du noeud de chemin XML path et les attributs attributes. Le noeud doit exister.
    void modify(const QString &path, const QString &value = "", const QMap<QString, QString> &attributes = QMap<QString,QString>());

    //! Test si le fichier peut être ouvert en écriture puis le referme.
    bool open() override;

    //! Lit le fichier de configuration et l'enregistre dans doc.
    void readConfIn(XmlDoc & doc);

    //! Modifie le chemin du fichier de configuration.
    void setFileName(const QString & fileName) override;

    //! Test s'il existe un noeud de chemin name.
    bool varExists(const QString & name);

    //bool varExists(const QString & name, const QMap<QString,QString> & atts  = QMap<QString,QString>());

    //! Recopie l'arbre XML vers le chemin path (utiliser plutôt copy).
    void recopie(const QString &path = QDir::homePath()+"/notecpp/configCopie.xml");

    //! Ecrit l'arbre doc dans le fichier de configuration.
    void writeConf(XmlDoc &doc);

protected:

    //! Ouvre le fichier en lecture, lance une exception si le fichier ne s'ouvre pas.
    void openRead()
    {
        if(!m_confFile.open(QFile::ReadOnly | QFile::Text))
            throw std::runtime_error("Imposible d'ouvrir pour lecture le fichier :"+m_confFile.fileName().toStdString());
    }

    //! Ouvre le fichier en écriture, lance une exception si le fichier ne s'ouvre pas.
    void openWrite()
    {
        if(!m_confFile.open(QFile::WriteOnly | QFile::Text))
            throw std::runtime_error("Imposible d'ouvrir pour ecriture le fichier :"+m_confFile.fileName().toStdString());
    }

    //! Referme le fichier de configuration.
    void close()
        {m_confFile.close();}
};

#endif // CONFIG_H
