#include "AbstractNoyau.h"

using namespace fenMPS;
using entity = entities::entity;

void AbstractNoyau::importXml(const QString &path) {
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        throw std::runtime_error("Imposible d'ouvrir pour lecture le fichier :"+file.name().toStdString());
    fichierMPS::reader_xml xml(&file);
    auto controle = m_bdd->importXml(xml.read());
    file.close();
    if(controle.isEmpty())
        QMessageBox::information(nullptr,tr("Importation de données à partir d'un fichier xml"),tr("Importation terminer succés"));
    else
        QMessageBox::information(nullptr,tr("Importation de données à partir d'un fichier xml"),tr("Échec de l'importation sur l'élément")
                                 .append("\n")
                                 .append(controle));
}

void AbstractNoyau::exportXml(const QString &path, conteneurMPS::vector_ptr<entities::entity> &&vec, flag option) {
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        throw std::runtime_error("Imposible d'ouvrir pour lecture le fichier :"+file.name().toStdString());
    fichierMPS::writer_xml xml(&file);
    xml.write(bdd().exportXml(std::move(vec),option));
    file.close();
}

void AbstractNoyau::schemaXmlForimport(const QString &path) {
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
        throw std::runtime_error("Imposible d'ouvrir pour écriture le fichier :"+file.name().toStdString());
    fichierMPS::writer_xml xml(&file);
    xml.write(m_bdd->schemaXmlForImport());
}

void AbstractNoyau::setBdd(std::unique_ptr<bmps::Bdd> &&bdd, const QString &pathXML, QWidget *modalParent) {
    m_bdd = std::move(bdd);
    //m_bdd->set_name(m_config.get_vars(DEFAULT_DIRECTORY));
    FenFoundFile fenBdd(*m_bdd,modalParent);
    fenBdd.openInConf(*m_config,pathXML);
    //m_bdd->miseAJourBdd();
}

void AbstractNoyau::setConfig(std::unique_ptr<fichierMPS::Config> &&config, QWidget *modalParent) {
    m_config = std::move(config);
    FenFoundFile fenConfig(*m_config,modalParent);
    fenConfig.exists();
    fenConfig.is_valid();
}

