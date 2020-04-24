#pragma once
#include "agent.hpp"
#include "wall.hpp"
#include <vector>
#include "QtDebug"
class MAPPGridState
{
    /*
    *   The grid state represents the actual state of the grid. It needs a list
    *   of agents, a list of wall positions, the x size of the grid and the
    *   y size of the grid.
    */
    public:
            static std::vector<Wall>walls;
            MAPPGridState( std::vector<Agent>&agents, unsigned int xsize, unsigned int ysize, unsigned int cost );
            inline unsigned int getF() const
            {
                return currentCost + currentHeuristic;
            }
            inline unsigned int getH() const
            {
                return currentHeuristic;
            }
            inline void setCost( unsigned int c ){ currentCost = c; }
            inline unsigned int getXsize()const{ return xsize; }
            inline unsigned int getYsize()const{ return ysize; }

            static bool hasWallAt( int x, int y )
            {
                /*
                * Check if there are walls at specific location
                */
                for( const auto &i : walls )
                {
                    if( i.getX() == x && i.getY() == y )
                    {
                        return true;
                    }
                }
                return false;
            }

            void succCoords( std::vector<Agent> &ret, const Agent &a ) const;

            inline bool sameCoord( const Agent &a, const Agent &b )
            {
                /*
                 * Test if two agents have the same coordinates 
                 */ 
                if( ( a.getX() == b.getX() ) && ( a.getY() == b.getY() ) )
                {
                    return true;
                }
                return false;
            }

            bool goodSuccessor( const std::vector<Agent> &successor );

            std::vector<MAPPGridState> successors();

            void show() const;

            bool operator ==( const MAPPGridState &a)const;
        
        friend struct std::less<MAPPGridState>;
        friend struct std::hash<MAPPGridState>;
        std::vector<Agent>agents;
        private:
        unsigned int xsize, ysize, numberAgents, currentCost, currentHeuristic;
};


namespace std
{
    /* Template specialisation for MAPPGridState priority_queue comparison */
    template<>
    struct less<MAPPGridState>
    {
        bool operator()( MAPPGridState const &s1, MAPPGridState const &s2 )
        {
            return s1.getF() > s2.getF();
        }
    };
    
    /* Template specialisation for MAPPGridState hash function*/
    template<>
    struct hash<MAPPGridState>
    {
        size_t operator()(const MAPPGridState &k) const
        {
            size_t hash = 0;
            unsigned int xsize = k.getXsize();
            unsigned int ysize = k.getYsize();
            for( const Agent &agent : k.agents )
            {
                hash = hash * (xsize * ysize) + agent.getX() + xsize * agent.getY();
            }
            return hash;
        }
    };
}