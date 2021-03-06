#include "Config.h"

using namespace fichierMPS;

Config::~Config() {
    if(m_confFile.isOpen())
        m_confFile.close();
}

void Config::add(const QString & path, const QString & value, const std::map<QString, QString> & attributes) {
    XmlDoc doc;
    readConfIn(doc);
    auto iter = doc.seek(path);
    if(iter) {
        iter->setText(value);
        iter->setAttributes(attributes);
        writeConf(doc);
    }
    else
        throw std::invalid_argument("void Config::add(const QString & path, const QString & value, const std::map<QString, QString> & attributes) \n "
                                    "path invalide, le premier terme du chemin doit être la racine du fichier de configuration.\n"
                                    " path = "+path.toStdString());
}

bool Config::copy(const QString &name) {
    Config file(name);
    if(file.exists() && file.isValid())
        return file.m_confFile.copy(m_fileName);
    else
        return false;
}

//bool Config::creer()
//{
//    XmlDoc doc;
//    XmlElement elt;
//    elt.setName("conf");
//    doc.addChild(elt);
//    elt.setName("directories");
//    doc.addChild(elt);
//    elt.setName("default");
//    elt.setText(QDir::homePath());
//    doc.addChild(elt);
//    doc.toParent();
//    doc.toParent();
//    elt.setName("files");
//    elt.setText("");
//    doc.addChild(elt);
//    elt.setName("databases");
//    doc.addChild(elt);

//    writeConf(doc);
//    return true;
//}

QString Config::getVars(const QString & name) {
    openRead();
    XmlReader xml(&m_confFile);
    auto result = xml.getVars(name).front();
    close();
    return result;
}

/*
QStringList Config::getVars(const QString & name, const QString & attName, const QString & attValue)
{
    openRead();
    XmlReader xml(&m_confFile);
    QStringList result = xml.getVars(name, attName, attValue);
    close();
    return result;
}

QStringList Config::getVars(const QString & name, const std::map<QString,QString> & atts)
{
    openRead();
    XmlReader xml(&m_confFile);
    QStringList result = xml.getVars(name, atts);
    close();
    return result;
}

std::vector<varAtts> Config::getVarsAtts(const QString & name, const QString & attName, const QString & attValue)
{
    openRead();
    XmlReader xml(&m_confFile);
    std::vector<varAtts> result = xml.getVarsAtts(name, attName, attValue);
    close();
    return result;
}

std::vector<varAtts> Config::getVarsAtts(const QString & name, const std::map<QString,QString> & atts)
{
    openRead();
    XmlReader xml(&m_confFile);
    std::vector<varAtts> result = xml.getVarsAtts(name, atts);
    close();
    return result;
}
*/

std::vector<varAtts> Config::getVarsAtts(const QString & name) {
    openRead();
    XmlReader xml(&m_confFile);
    auto result = xml.getVarsAtts(name);
    close();
    return result;
}

bool Config::isValid() {
    openRead();
    QXmlStreamReader xml(&m_confFile);
    if (xml.readNextStartElement())
        if (xml.name() != "conf")
                xml.raiseError(QObject::tr("Le fichier config.xml n'est pas valide"));
    close();
    return !xml.error();

}

void Config::modify(const QString &path, const QString &value, const std::map<QString, QString> &attributes) {
    XmlDoc doc;
    readConfIn(doc);
    auto iter = doc.seek(path);
    if(iter) {
        iter->setText(value);
        iter->setAttributes(attributes);
        writeConf(doc);
    }
    else
        throw std::invalid_argument("void Config::modify(const QString & path, const QString & value, const std::map<QString, QString> & attributes) \n "
                                    "path invalide, le premier terme du chemin doit être la racine du fichier de configuration.\n"
                                    " path = "+path.toStdString());
}

bool Config::open() {
    bool test = m_confFile.open(QFile::ReadWrite);
    m_confFile.close();
    return test;
}

void Config::readConfIn(XmlDoc & doc) {
    openRead();
    XmlReader xmlR(&m_confFile);
    xmlR.readIn(doc);
    close();
}

void Config::setFileName(const QString & fileName) {
    FileInterface::setFileName(fileName);
    if(m_confFile.isOpen())
        m_confFile.close();
    m_confFile.setFileName(fileName);
}

bool Config::varExists(const QString & name) {
    openRead();
    XmlReader xml(&m_confFile);
    auto result = xml.exists(name);
    close();
    return result;
}

/*
bool Config::varExists(const QString & name, const QString & attName, const QString & attValue)
{
    openRead();
    XmlReader xml(&m_confFile);
    bool result = xml.exists(name, attName, attValue);
    close();
    return result;
}

bool Config::varExists(const QString &name, const std::map<QString, QString> &atts)
{
    openRead();
    XmlReader xml(&m_confFile);
    bool result = xml.exists(name, atts);
    close();
    return result;

}
*/

void Config::recopie(const QString & path) {
    openRead();
    XmlReader xmlR(&m_confFile);
    XmlDoc doc(xmlR.read());
    close();
    QFile output(path);
    output.open(QFile::WriteOnly | QFile::Text);
    XmlWriter xmlW(&output);
    xmlW.write(doc);
    output.close();
}

void Config::writeConf(XmlDoc & doc) {
    openWrite();
    XmlWriter xmlW(&m_confFile);
    xmlW.write(doc);
    close();
}
