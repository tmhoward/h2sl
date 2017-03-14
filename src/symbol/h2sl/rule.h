/**
 * @file    rule.h
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 *
 * The interface for a class used to describe a rule
 */

#ifndef H2SL_RULE_H
#define H2SL_RULE_H

#include <iostream>
#include <vector>
#include <map>
#include <libxml/tree.h>

#include "h2sl/grounding.h"

namespace h2sl {
  class World;

  class Rule : public Grounding {
  public:
    Rule( const std::map< std::string, std::string >& stringProperties = std::map< std::string, std::string >(), const std::map< std::string, int >& intProperties = std::map< std::string, int >() );
    virtual ~Rule();
    Rule( const Rule& other );
    Rule& operator=( const Rule& other );
    bool operator==( const Rule& other )const;
    bool operator!=( const Rule& other )const;

  protected:

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Rule& other );
}

#endif /* H2SL_RULE_H */
