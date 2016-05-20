/*
 * <TRV pathfinding solution using genetic algorithm>
 * Copyright (C) 2016  <copyright Roux Nicolas, Voillemin Théo, Talarmain Corentin>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*!
 *\file algogen.h
 *\author Nicolas Roux, Théo Voillemin, Corentin Talarmain
 *\brief Algogen's file
 * 
 */

#ifndef ALGOGEN_H
#define ALGOGEN_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <ctime>
#include <cstdlib>
#include <math.h>

#include "sousminion.h"
#include "surminion.h"
#include "str_exception.h"
#include "map.h"
#include "zone.h"
#include <unordered_map>

/*!
 * \brief fonction comparaison of SurMinion
 * Compare two SurMinion for the sort of the population
 * 
 * \param _m1 : The fisrt SurMinion to compare
 * \param _m2 : The second SurMinion to compare
 */
 
inline bool myfonction(SurMinion *_m1, SurMinion *_m2)
{
    if(_m1->getVaChemin() == _m2->getVaChemin()){
      return (_m1->getFitness()<_m2->getFitness());
    }else if(_m1->getVaChemin() < _m2->getVaChemin()){
      return 0;
    }else{
      return 1;
    }
}

class Algogen{

/*! \class Algogen
 * \brief Class representing the treatment of the genetic algorithm
 * This class handle the genetic algorithm
 */

private:
	int m_mapW, m_mapH;  // Map width, map height
	std::unordered_map<int, std::pair<int,int> > m_prochCases;
	std::map<int, Case*> const * m_sommets; // Pointer to the map's tiles container
	std::vector<Case *>m_orig,m_cible; // Pointers to the requested beginning/ending tiles
	std::vector<SurMinion*> m_pop; // Population of individuals, each of them representing every requested paths (see class SurMinion)
	std::vector<SousMinion*> m_sousMinions; // "sub"-individuals, representing the path taken by units to fall in line with the leader of a group
	std::vector<const Unite*> m_unite; // Storage of unit types, used to get information about the speed and collision parameters of a type of unit
	std::vector<Zone*>m_zones; // Contains the areas subject to falling in line behind a leader rather than finding a new path
	SurMinion* m_president, *m_superman; // Special individuals, respectively the best individual yet (the one whose path is returned, not affected by mutations/deletions/etc) and one that is created by selecting the best paths of all individuals
	unsigned int m_popsize, m_taillemax; // Population size and maximum genome length (!)
	unsigned int m_nbkids; // Number of children generated by every triplet of parents
	unsigned int m_lowestElite; // Not a parameter, depends on ratioElitism and m_popsize
	unsigned int m_nbkidstotal, m_nbIterations; // Total number of children created and iterations done, for reporting purposes
	float m_manhattanImportance; // Relative importance of manhattan distance in fitness formula
	float m_mutationRatio; // Mutation ratio per individual and per generation
	float m_popToMutate; // Population ratio subject to mutations
	unsigned int m_nbAjouts; // Numbre of added chromosomes per generation (for relevant individuals)
	float m_ratioSupprs, m_ratioModifs, m_initratioSupprs,m_initratioModifs; // Initial ratios of chromosomes deletion and modification (subject to change over time, cf adaptive genetic algorithm)
	float m_ratioElitism; // Ratio of individuals considered "elite" (selection by fitness) 
	float m_cullRatio; // Ratio of individuals deleted at each iteration
	std::vector<float> m_generationTotalFitness; // Contains each iteration's global fitness, used to adapt m_ratioSupprs and m_ratioModifs
	std::vector<std::pair<unsigned int,unsigned int>> m_conf_pres; // Container used for collision detection and subsequent genome modifications
	unsigned int m_nbChemins;
	
	
	/*!
	 * \brief Initiation of population
	 * 
	 * Initiate the population each time we ask a new treatment of path per agent. The function add new Minions to the SurMinion of the population.
	 * 
	 * \param _caseSource : The requested beginning tiles.
	 * \param _caseCible : The requested ending tiles.
	 * \param _typeAgent : The unit type of the agent.
	 * \param _idAgent : The ID of the agent.
	 */
	 
	void initPop(int _caseSource, int _caseCible,const Unite * _typeAgent, int _idAgent); // Creation of a new sub-population in each individual, at the request of demandeDéplacement()
	
	public:
	
	
	  /*!
	   * \brief Constructor
	   * 
	   * Construct an Algogen with all the parameters.
	   * 
	   * \param map_w : The width of the map.
	   * \param map_h : The height of the map.
	   * \param _sommets : Pointer to the map's tiles container.
	   * \param _popsize : Population maximum size.
	   * \param _manhattanImportance : Relative importance of manhattan distance in fitness formula.
	   * \param _mutationRatio : Mutation ratio per individual and per generation.
	   * \param _popToMutate : Population ratio subject to mutations.
	   * \param _nbAjouts : Numbre of added chromosomes per generation (for relevant individuals).
	   * \param _ratioSupprs : Initial ratios of chromosomes deletion
	   * \param _ratioModifs : Initial ratios of chromosomes modification
	   * \param _ratioElitism : Ratio of individuals considered "elite"
	   * \param _cullRatio : Ratio of individuals deleted at each iteration.
	   * \param _nbkids : Number of children generated by every triplet of parents.
	   */
	   
	Algogen(int map_w, int map_h,std::map<int, Case*> const * _sommets, unsigned int _popsize, float _manhattanImportance, float _mutationRatio, 
		  float _popToMutate, unsigned int _nbAjouts, float _ratioSupprs,float _ratioModifs, float _ratioElitism, float _cullRatio, unsigned int _nbkids);
	
	
	/*!
	 * \brief Destructor
	 * 
	 * Destruct the Algogen and all it vector of pointers (SurMinion, Zone and SousMinion).
	 */
	 
	~Algogen();


	/*!
	 * \brief addDeplacement
	 * 
	 * Called by the map: processes a new path request.
	 * 
	 * \param _idAgent : The id of the Agent.
	 * \param _caseSource : The requested beginning tiles.
	 * \param _caseCible : The requested ending tiles.
	 * \param _typeAgent : The unit type of the agent.
	 */

	void addDeplacement(int _idAgent, int _caseSource, int _caseCible, const Unite * _typeAgent);
	
	
	/*!
	 * \brief iterate
	 * 
	 * Main method, calling all genetic algorithm functions. This method should be called a number of times dependant on the map size.
	 */
	
	void iterate();
	
	
	/*!
	 * \brief calcSousMinions
	 * 
	 * In real time process, calculate the next move of each agent.
	 */
	
	void calcSousMinions();
	
	
	/*!
	 * \brief crossover
	 * 
	 * Creates new individuals based on the genome of three parents.
	 * 
	 * \param _parent0 : Pointer of the first SurMinion parent
	 * \param _parent1 : Pointer of the second SurMinion parent
	 * \param _parent2 : Pointer of the third SurMinion parent
	 */
	
	void crossover(SurMinion* _parent0, SurMinion* _parent1, SurMinion* _parent2);
	
	
	/*!
	 * \brief mutatePop
	 * 
	 * Mutates the population (see .cpp for details).
	 */
	
	void mutatePop(); // Mutates the population (see .cpp for details)
	
	
	/*!
	 * \brief cull
	 * 
	 * Destroys a number of individuals (see .cpp for details).
	 */
	
	void cull();
	
	
	/*!
	 * \brief evaluate
	 * 
	 * Assigns a fitness to an individual as a whole, and to each of its paths. Corrects loops, out of bounds movements, overshootings, and also manages collisions.
	 *
	 * \param _minion : Pointer of the SurMinion to evaluate
	 */
	
	void evaluate(SurMinion* _minion);
	
	
	/*!
	 * \brief evaluateSSM
	 * 
	 * Manages collisions for sub-individuals.
	 */
	
	void evaluateSSM();
	
	
	/*!
	 * \brief show
	 * 
	 * prints statistics and details about the results.
	 */
	
	void show() const;
	
	
	/*!
	 * \brief calcule_Zone
	 * 
	 * \return The new Zone create.
	 * 
	 * When a group of Agent are near to each other and has the same destination, create a Zone* with one leader wich the other agents will follow.
	 *
	 * \param _caseSource : The requested beginning tiles.
	 * \param _caseCible : The requested ending tiles.
	 */
	
	Zone *calcule_Zone(int _caseSource, int _caseCible);
	
	
	/*!
	 * \brief getProchCase
	 * 
	 * \return A pair of the coordinate of the next case.
	 * 
	 * Getter of the value of the vector m_prochCases for an agent.
	 *
	 * \param _agentID : The ID of the agent.
	 */
	
	std::pair<int,int> getProchCase(int _agentID) const;
	
	
	/*!
	* \brief Getter on m_nbChemins
	*
	* \return The value of m_nbChemins
	*/
	
	unsigned int getNbChemins();
	
	
	/*!
	 * \brief move_agent
	 * 
	 * Change the vector m_orig with the new beginning tiles after moving an agent.
	 *
	 * \param id : The ID of the agent.
	 * \param x : The abscissa of the new beginning tiles.
	 * \param y : The ordinate of the new beginning tiles.
	 */	
	
	void move_agent(int id, int x, int y);
};

#endif
