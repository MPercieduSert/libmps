/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef XML_H
#define XML_H

/*! \defgroup groupeXML XML
 *
 *\brief Ensemble de classes utiliser pour la manipulation de fichiers XML.
 */

#include <map>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <vector>
#include "tree.h"

namespace mps {
namespace fichier {

/*! \var typedef std::pair<QString,std::map<QString,QString> > val_atts_t;
 *\brief Couple (Valeur,Attribut sous forme liste<clé,valeur>) d'un noeud
 */
using val_atts_t = std::pair<QString,std::map<QString,QString>>;

/*! \ingroup groupeXML
 *\brief Cette classe est un noeud d'un document XML.
 *
 *Cette classe est un noeud d'arbre permettant de manipuler un document XML avec la classe doc_xml.
 *\sa doc_xml
 */
class element_xml {
protected:
    QString m_name;                         //!< Nom du noeuds XML.
    QString m_text;                         //!< Texte contenue entre les deux balises du noeuds XML.
    std::map<QString,QString> m_attributes; //!< Attributs XML sous la forme <clé,valeur> attachés au noeud.

public:
    //! Constructeur.
    element_xml()
        {}

    //! Constructeur.
    element_xml(const QString &name)
        {m_name = name;}

    //! Constructeur.
    element_xml(const char *name)
        {m_name = name;}

    //! Renvoie les attributs du noeud sous la forme d'un map<clé,valeur>.
    const std::map<QString,QString> &attributes() const
        {return m_attributes;}

    //! Test si le noeud posséde des attributs.
    bool has_attributes() const
        {return !m_attributes.empty();}

    //! Test si le noeud contient du texte.
    bool has_text() const
        {return !m_text.isEmpty();}

    //! Renvoie le nom de du noeud.
    const QString &name() const
        {return m_name;}

    //! Ajoute ou modifie l'attribut de nom name avec la valeur val.
    void set_attribut(const QString &name, const QString &val)
        {m_attributes[name] = val;}

    //! Remplace les attributs du noeud par ceux donnés en argument sous la forme d'un std::map<clé,valeur>.
    void set_attributes(const std::map<QString,QString> &map)
        {m_attributes = map;}

    //! Modifie le nom du noeud.
    void set_name(const QString &name)
        {m_name = name;}

    //! Modifie le texte du noeud courant.
    void set_text(const QString &text)
        {m_text = text;}

    //! Renvoie le texte du noeud.
    const QString &text() const
        {return m_text;}
};

/*! \ingroup groupeXML
 *\brief  Cette classe est un arbre de element_xml permettant de manipuler un document XML.
 *
 *Le document XML est répresenté par la classe doc_xml sous la forme un arbre de element_xml.
 *La navigation dans l'arbre s'effectue à l'aide d'un noeud courant tel un curseur.
 *Voir la classe Tree pour connaitre la manière de parcourir l'arbre, de modifier ou de créer des noeuds dans l'arbre.
 * - Un doc_xml peut-être obtenu à partir d'un fichier XML grâce à reader_xml.
 * - Un doc_xml peut-être écrit dans un fichier XML grâce à writer_xml.
 *\sa tree, element_xml, reader_xml, writer_xml
 */
class doc_xml : public tree<element_xml> {
public:
    //! Constructeur.
    doc_xml() = default;
    
    /*! \brief Renvoie un itérateur sur le noeud de chemin indiqué.
     *L'itérateur pointe sur l'élément correspond au chemin indiqué dont les différents noeuds sont
     *séparés par un /. Si le premier terme du chemin ne coorespond pas à la racine, l'itérteur renvoyé est nul
     *dans le cas contraire, si les noeuds du chemin n'existent pas il sont créés.
     */
    prevsuiv_iterator seek(const QString &chemin);

    //! Revoie l'arbre dévelopé sous forme d'un QString.
    QString to_string() const;
};

/*! \ingroup groupeXML
 *\brief Cette classe permet de lire un document XML.
*
*Cette classe donne accés en lecture aux noeuds d'un document XML
*ou permet d'obtenir l'arbre des noeuds sous la forme d'un doc_xml.
*Tous les chemins sont relatifs.
*\sa element_xml, doc_xml, reader_xml
*/
class reader_xml : public QXmlStreamReader {
public:
    using QXmlStreamReader::QXmlStreamReader;

    //! Test si le noeuds dont le chemin est donné par name et possédant l'attribut attName de valeur attValue existe.
    bool exists(const QString &name, const QString &att_name, const QString &att_value) {
        std::map<QString,QString> atts;
        atts.insert({att_name,att_value});
        return exists(name, atts);
    }

    //! Test si le noeuds dont le chemin est donné par name
    //! et possédant les attributs atts donné sous forme d'un std::map<clé,valeur> existe.
    bool exists(QString name, const std::map<QString,QString> &atts = std::map<QString,QString>());

    //! Retourne la liste des valeurs des noeuds dont le chemin est donné par name
    //! et possédant l'attribut attName de valeur attValue.
    std::list<QString> get_vars(const QString &name, const QString &att_name, const QString &att_value) {
        std::map<QString,QString> atts;
        atts.insert({att_name,att_value});
        return get_vars(name, atts);
    }

    //! Retourne la liste des valeurs des noeuds dont le chemin est donné par name 
    //! et possédant les attributs atts donné sous forme d'un std::map<clé,valeur>.
    std::list<QString> get_vars(QString name, const std::map<QString,QString> &atts = std::map<QString,QString>());

    //! Retourne un vecteur de pairs (valeur,attributs) associés aux noeuds dont le chemin est donné par name 
    //! et possédant l'attribut attName de valeur attValue.
    std::vector<val_atts_t> get_vars_atts(const QString &name, const QString &attName, const QString &attValue) {
        std::map<QString,QString> atts;
        atts.insert({attName,attValue});
        return get_vars_atts(name, atts);
    }

    //! Retourne un vecteur de pairs (valeur,attributs) associés aux noeuds dont le chemin est donné par name 
    //! et possédant les attributs atts donné sous forme d'un std::map<clé,valeur>.
    std::vector<val_atts_t> get_vars_atts(QString name, const std::map<QString,QString> &atts = std::map<QString,QString>());

    //! Lit entièrement le fichier Xml et stoke dans l'arbre doc_xml transmis en argument.
    void read_in(doc_xml &doc);

    //! Lit entièrement le fichier Xml puis renvoie un arbre doc_xml.
    doc_xml read();

    //! Lit le noeud courant du flux QXmlStreamReader puis retourne un element_xml contenant les informations du noeud.
    element_xml read_element() const;
protected:
    //! Place le curseur sur l'élément du chemin path en laissant le dernier élément dans path.
    //! Retourne true si l'élément est trouvé.
    bool to_path(QString &path);
};

/*! \ingroup groupeXML
 *\brief Cette classe permet d'écrire un document XML à partir d'un arbre doc_xml.
 */
class writer_xml : public QXmlStreamWriter {
public:
    using QXmlStreamWriter::QXmlStreamWriter;

    //! Fonction écrit l'arbre doc_xml transmis en argument dans le fichier attaché au flux QXmlStreamWriter.
    void write(const doc_xml &doc);
};
}}
#endif // XML_H
