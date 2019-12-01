#include "AbstractNoyau.h"

using namespace fenMPS;

void AbstractNoyau::setBdd(std::unique_ptr<bmps::Bdd> && bdd, const QString &pathXML, QWidget * modalParent) {
    m_bdd = std::move(bdd);
    //m_bdd->setFileName(m_config.getVars(DEFAULT_DIRECTORY));
    FenFoundFile fenBdd(*m_bdd,modalParent);
    fenBdd.openInConf(*m_config,pathXML);
    //m_bdd->miseAJourBdd();
}

void AbstractNoyau::setConfig(std::unique_ptr<fichierMPS::Config> &&config, QWidget * modalParent) {
    m_config = std::move(config);
    FenFoundFile fenConfig(*m_config,modalParent);
    fenConfig.exists();
    fenConfig.isValid();
}
