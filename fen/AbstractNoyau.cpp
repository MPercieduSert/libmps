#include "AbstractNoyau.h"

void AbstractNoyau::setBdd(Bdd * bdd, const QString &pathXML, QWidget * modalParent)
{
    m_bdd = bdd;
    //m_bdd->setFileName(m_config->getVars(DEFAULT_DIRECTORY));
    FenFoundFile fenBdd(m_bdd,modalParent);
    fenBdd.openInConf(m_config,pathXML);
    //m_bdd->miseAJourBdd();
}

void AbstractNoyau::setConfig(Config * config, QWidget * modalParent)
{
    m_config = config;
    FenFoundFile fenConfig(m_config,modalParent);
    fenConfig.exists();
    fenConfig.isValid();
}
