#include "abstract_noyau.h"

using namespace mps;
using namespace fenetre;
using entity = entities::entity;

void abstract_noyau::import_xml(const QString &path) {
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        throw std::runtime_error("Imposible d'ouvrir pour lecture le fichier :"+file.fileName().toStdString());
    fichier::reader_xml xml(&file);
    auto controle = m_bdd->import_xml(xml.read());
    file.close();
    if(controle.isEmpty())
        QMessageBox::information(nullptr,tr("Importation de données à partir d'un fichier xml"),
                                 tr("Importation terminer succés"));
    else
        QMessageBox::information(nullptr,tr("Importation de données à partir d'un fichier xml"),
                                 tr("Échec de l'importation sur l'élément")
                                 .append("\n")
                                 .append(controle));
}

void abstract_noyau::export_xml(const QString &path, vector_ptr<entities::entity> &&vec, flag option) {
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        throw std::runtime_error("Imposible d'ouvrir pour lecture le fichier :"+file.fileName().toStdString());
    fichier::writer_xml xml(&file);
    xml.write(bdd().export_xml(std::move(vec),option));
    file.close();
}

void abstract_noyau::schema_xml_for_import(const QString &path) {
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        throw std::runtime_error("Imposible d'ouvrir pour écriture le fichier :"+file.fileName().toStdString());
    fichier::writer_xml xml(&file);
    xml.write(m_bdd->schema_xml_for_import());
}

void abstract_noyau::set_bdd(std::unique_ptr<b2d::bdd> &&bdd, const QString &path_xml, QWidget *modal_parent) {
    m_bdd = std::move(bdd);
    fen_found_file fen_bdd(*m_bdd,modal_parent);
    fen_bdd.open_in_conf(*m_config,path_xml);
}

void abstract_noyau::set_config(std::unique_ptr<fichier::config> &&config, QWidget *modal_parent) {
    m_config = std::move(config);
    fen_found_file fen_config(*m_config,modal_parent);
    fen_config.exists();
    fen_config.is_valid();
}
