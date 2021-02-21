#include "config.h"

using namespace mps::fichier;

config::~config() {
    if(m_file.isOpen())
        m_file.close();
}

void config::add(const QString &path, const QString &value, const std::map<QString, QString> &attributes) {
    if(!set_attribut(path,value,attributes))
        throw std::invalid_argument("void config::add(const QString &path, const QString &value, const std::map<QString, QString> &attributes) \n "
                                    "path invalide, le premier terme du chemin doit être la racine du fichier de configuration.\n"
                                    " path = "+path.toStdString());
}

bool config::copy(const QString &name) {
    config file(name);
    if(file.exists() &&file.is_valid())
        return file.m_file.copy(m_name);
    else
        return false;
}

QString config::get_vars(const QString &name) {
    open_read();
    reader_xml xml(&m_file);
    auto result = xml.get_vars(name).front();
    close();
    return result;
}

std::vector<val_atts_t> config::get_vars_atts(const QString &name) {
    open_read();
    reader_xml xml(&m_file);
    auto result = xml.get_vars_atts(name);
    close();
    return result;
}

bool config::is_valid() {
    open_read();
    QXmlStreamReader xml(&m_file);
    if (xml.readNextStartElement())
        if (xml.name() != "conf")
                xml.raiseError(QObject::tr("Le fichier config.xml n'est pas valide"));
    close();
    return !xml.error();

}

void config::modify(const QString &path, const QString &value, const std::map<QString, QString> &attributes) {
    if(!set_attribut(path,value,attributes))
        throw std::invalid_argument("void config::modify(const QString &path, const QString &value, const std::map<QString, QString> &attributes) \n "
                                    "path invalide, le premier terme du chemin doit être la racine du fichier de configuration.\n"
                                    " path = "+path.toStdString());
}

bool config::open() {
    bool test = m_file.open(QFile::ReadWrite);
    m_file.close();
    return test;
}

void config::read_conf_In(doc_xml &doc) {
    open_read();
    reader_xml xmlR(&m_file);
    xmlR.read_in(doc);
    close();
}

bool config::set_attribut(const QString &path, const QString &value ,
                  const std::map<QString, QString> &attributes) {
    doc_xml doc;
    read_conf_In(doc);
    auto it = doc.seek(path);
    if(it) {
        it->set_text(value);
        it->set_attributes(attributes);
        write(doc);
        return true;
    }
    return false;
}

void config::set_name(const QString &name) {
    file_interface::set_name(name);
    if(m_file.isOpen())
        m_file.close();
    m_file.setFileName(name);
}

bool config::var_exists(const QString &name) {
    open_read();
    reader_xml xml(&m_file);
    auto result = xml.exists(name);
    close();
    return result;
}

void config::recopie(const QString &path) {
    open_read();
    reader_xml xmlR(&m_file);
    doc_xml doc(xmlR.read());
    close();
    QFile output(path);
    output.open(QFile::WriteOnly | QFile::Text);
    writer_xml xmlW(&output);
    xmlW.write(doc);
    output.close();
}

void config::write(doc_xml &doc) {
    open_write();
    writer_xml xmlW(&m_file);
    xmlW.write(doc);
    close();
}
