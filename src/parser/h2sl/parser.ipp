/**
 * @file    parser.ipp
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
 * The implementation of a class that describes a parser
 */

template< class T >
Parser<T>::
Parser() {

}

template< class T >
Parser<T>::
~Parser() {

}

template< class T >
Parser<T>::
Parser( const Parser<T>& other ) {

}

template< class T >
Parser<T>&
Parser<T>::
operator=( const Parser<T>& other ) {

  return (*this);
}

template< class T >
void
Parser<T>::
_text_to_words( const std::string& text,
                std::vector< Word >& words )const{
  std::string tmp_text = boost::replace_all_copy( text, ",", "" );
  std::vector< std::string > tmp_words;
  boost::split( tmp_words, tmp_text, boost::is_any_of( " " ) );

  words.clear();
  for( unsigned int i = 0; i < tmp_words.size(); i++ ){
    words.push_back( Word( POS_UNKNOWN, tmp_words[ i ], i ) );
  }
  return;
}

template< class T >
std::ostream&
operator<<( std::ostream& out,
            const Parser<T>& other ) {
  return out;
}

