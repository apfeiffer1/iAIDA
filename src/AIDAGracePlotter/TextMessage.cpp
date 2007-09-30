//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

// -*- C++ -*-

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  This library is free software; you can redistribute it and/or      //
//  modify it under the terms of the GNU Lesser General Public         //
//  License as published by the Free Software Foundation; either       //
//  version 2.1 of the License, or (at your option) any later version. //
//                                                                     //
//  This library is distributed in the hope that it will be useful,    //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of     //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   //
//  Lesser General Public License for more details.                    //
//                                                                     //
//  You should have received a copy of the GNU Lesser General Public   //
//  License along with this library (see file COPYING); if not, write  //
//  to the Free Software Foundation, Inc., 59 Temple Place, Suite      //
//  330, Boston, MA 02111-1307 USA, or contact the author.             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

//
// (c) CERN, Geneva
//

//
// Implementation file for class TextMessage
// 
// Created by user sang on Thu Mar  7 23:13:28 2002
// Last update: Thu Mar  7 23:13:28 2002
// 


#include "TextMessage.h"
#include <iostream>



namespace AidaNative {




//////////////////////////
//                      //
// Const static members //
//                      //
//////////////////////////

const std::string           
  TextMessage::DEFAULT_PREFIX = "***";
const int
  TextMessage::MAX_TOKENS = 9;
const std::string           
TextMessage::TOKEN_DELIMITER[MAX_TOKENS] = {"$1", "$2", "$3", "$4", "$5", 
					    "$6", "$7", "$8", "$9"};
const TextMessage::Severity 
TextMessage::DEFAULT_SEVERITY = ERROR;
const TextMessage::Severity 
TextMessage::DEFAULT_VERBOSITY = INFO;
const std::string 
TextMessage::DEFAULT_MESSAGE = "[empty message]";




//////////////////////////
//                      //
// Const static members //
//                      //
//////////////////////////

std::ostream* TextMessage::theStream = &(::std::cout);
bool TextMessage::silent = false;
std::string TextMessage::prefix = DEFAULT_PREFIX;




////////////////////////////////////////////
//                                        //
// Constructors, destructor and operator= //
//                                        //
////////////////////////////////////////////

TextMessage::TextMessage()
  : m_severity(DEFAULT_SEVERITY),
    m_message (DEFAULT_MESSAGE),
    m_context ("")
{
  tokeniseMessage();
}





TextMessage::TextMessage(const std::string& msg, const std::string& context,
			 Severity sev)
  : m_severity(sev),
    m_message (msg),
    m_context (context)
{
  tokeniseMessage();
}




////////////////////
//                //
// Public methods //
//                //
////////////////////

void TextMessage::setSeverity(TextMessage::Severity newSeverity)
{
  m_severity = newSeverity;
}




void TextMessage::setMessage(const std::string& newMessage)
{
  m_message = newMessage;
  tokeniseMessage();
}




void TextMessage::setContext(const std::string& newContext)
{
  m_context = newContext;
}




///////////////////////////
//                       //
// Public static methods //
//                       //
///////////////////////////

void TextMessage::setStream(std::ostream& s)
{
  theStream = &s;
}




void TextMessage::disableMessages(void)
{
  silent = true;
}




void TextMessage::enableMessages(void)
{
  silent = false;
}




void TextMessage::setPrefix(const std::string& newPrefix)
{
  prefix = newPrefix;
}



/////////////////////
//                 //
// Private methods //
//                 //
/////////////////////

void TextMessage::writeContext(std::ostream& s) const
{
  s << prefix << " " << m_context << " "
    << severityString(m_severity) << ": ";
}



std::string TextMessage::severityString(const TextMessage::Severity& sev)
{
  switch (sev) {
  case DEBUG:   return "[DEBUG]";
  case INFO:    return "info";
  case WARNING: return "warning";
  case ERROR:   return "error";
  case SEVERE:  return "SEVERE ERROR";
  default:      return "error";
  }
}




void TextMessage::tokeniseMessage()
{
  messageTokens.clear();
  const std::string msg = m_message;

  if (msg.length() == 0) {
    return;
  }

  std::string token;
  typedef size_t St;
  const St startOfMessage = 0;
  const St endOfMessage = msg.length() - 1;
  St startOfToken = startOfMessage;
  St endOfToken   = startOfMessage;
  for (int i = 0; i < MAX_TOKENS; ++i) {
    endOfToken = msg.find(std::string(TOKEN_DELIMITER[i]), startOfToken);
    if (endOfToken == std::string::npos) endOfToken = endOfMessage;
    token = msg.substr(startOfToken, endOfToken-startOfToken);
    messageTokens.push_back(token);
    startOfToken = endOfToken + (TOKEN_DELIMITER[i]).length();
    if (endOfToken >= endOfMessage || startOfToken >= endOfMessage) break;
  }
}




} // end of namespace AidaNative
