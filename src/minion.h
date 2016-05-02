#ifndef MINION_H
#define MINION_H
#include <vector>
#include <iostream>

#include "case.h"
// #include "map.h"

class Minion{
private:
  static unsigned int m_incrID;
  std::vector<std::pair<bool,bool> > m_genome;
  float m_fitness;
  bool m_vaChemin;
  unsigned int m_manhattan, m_id,m_sommetfinal;
public:
  Minion(std::vector<std::pair<bool,bool> > const & _genome);
  void mutate(Case * _cible, unsigned int _nbAjouts, float _ratioSupprs,float _ratioModifs);
  void mutateElite(Case * _cible, unsigned int _nbAjouts,float _ratioModifs);
  
  unsigned int getManhattan() const;
  bool getVaChemin() const;
  float const & getFitness() const;
  std::vector<std::pair<bool,bool> > const & getGenome() const;
  unsigned int getGenomeSize() const;
  std::pair< bool, bool >& getChromosome(unsigned int _pairNumber);
  unsigned int getID() const;
  unsigned int getSF() const;
  
  inline bool operator< (const Minion& _m2){ 
    if((m_vaChemin && _m2.getVaChemin()) || (!m_vaChemin && !_m2.getVaChemin())){
      return m_fitness>_m2.getFitness();
    }else if(!m_vaChemin){
      return 0;
    }else{
      return 1;
    }
  }

  void setFitness(float _fitness);
  void setVaChemin(bool _vaChemin);
  void setManhattan(unsigned int _manhattan);
  void setGenome( std::vector<std::pair<bool,bool> > const & _genome);
  void setSommetFinal(unsigned int _sommet);
};

#endif