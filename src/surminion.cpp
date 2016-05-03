#include "surminion.h"
unsigned int SurMinion::m_incrID=0;

SurMinion::SurMinion(std::vector<Minion*> const & _minions):m_minions(_minions)
{
  m_vaChemin=false;
  m_id=m_incrID++;
}

void SurMinion::addMinion(Minion* _minion)
{
  m_minions.push_back(_minion);
}


const float& SurMinion::getFitness() const
{
  return m_fitness;
}

std::vector<Minion*> const & SurMinion::getMinions() const
{
  return m_minions;
}

unsigned int SurMinion::getNumberMinions() const
{
  return m_minions.size();
}

unsigned int SurMinion::getID() const
{
  return m_id;
}

Minion* SurMinion::getMinion(unsigned int _pairNumber)
{
  return m_minions.at(_pairNumber);
}

bool SurMinion::getVaChemin() const
{
  return m_vaChemin;
}

void SurMinion::setFitness(float _fitness)
{
  m_fitness = _fitness;
}

void SurMinion::setMinions(std::vector<Minion*> const & _minions)
{
  m_minions=_minions;
}

void SurMinion::setVaChemin(bool _vaChemin)
{
  m_vaChemin=_vaChemin;
}
void SurMinion::mutate(int numMinion, unsigned int _nbAjouts, float _ratioSupprs, float _ratioModifs)
{
  
}

void SurMinion::mutateElite(int numMinion, unsigned int _nbAjouts, float _ratioModifs)
{

}

