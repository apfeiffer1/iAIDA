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

#ifndef AIDANATIVE_TEXT_MESSAGE_H
#define AIDANATIVE_TEXT_MESSAGE_H




#include <vector>
#include <string>

#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif



namespace AidaNative {



/**
 * Encapsulates a message for the end user as an object. Several severities
 * permitted (Error, warning etc). Substitutes ints, doubles, strings etc in
 * appropriate places marked with $1, $2 and so on. Example:
   <pre>
   static const TextMessage fileNotFound("sorry, file '$1' not found");
   ...
   if (...) fileNotFound.show(filename);
   </pre>
 * It's templated so it can handle up to 9 arguments of any type 
 * which has an operator<<(). 
 */

class TextMessage
{
public:
  enum Severity {DEBUG, INFO, WARNING, ERROR, SEVERE};
  
public:
  TextMessage();
  TextMessage(const std::string& messageText, const std::string& context = "",
	      Severity = ERROR);
  
public:
  /// Set the severity of this message.
  void setSeverity(Severity newSeverity);
  /// Set the message text (use the $N syntax for substitutions).
  void setMessage(const std::string& newMessage);
  /// Set the current 'context string' which gets printed with this message.
  void setContext(const std::string& newContext = "");
  inline void show() const;
  template<class T1>
    inline void show(T1 a) const;
  template<class T1, class T2>
    inline void show(T1 a, T2 b) const;
  template<class T1, class T2, class T3>
    inline void show(T1 a, T2 b, T3 c) const;
  template<class T1, class T2, class T3, class T4>
    inline void show(T1 a, T2 b, T3 c, T4 d) const;
  template<class T1, class T2, class T3, class T4, class T5>
    inline void show(T1 a, T2 b, T3 c, T4 d, T5 e) const;
  template<class T1, class T2, class T3, class T4, class T5, 
	   class T6>
    inline void show(T1 a, T2 b, T3 c, T4 d, T5 e, 
		     T6 f) const;
  template<class T1, class T2, class T3, class T4, class T5, 
	   class T6, class T7>
    inline void show(T1 a, T2 b, T3 c, T4 d, T5 e, 
		     T6 f, T7 g) const;
  template<class T1, class T2, class T3, class T4, class T5, 
	   class T6, class T7, class T8>
    inline void show(T1 a, T2 b, T3 c, T4 d, T5 e, 
		     T6 f, T7 g, T8 h) const;
  /// Show the message on the current stream.
  template<class T1, class T2, class T3, class T4, class T5, 
	   class T6, class T7, class T8, class T9>
    inline void show(T1 a, T2 b, T3 c, T4 d, T5 e, 
		     T6 f, T7 g, T8 h, T9 i) const;

public:
  /// Set the output stream for all messages.
  static void setStream(std::ostream&);
  /// Enable all messages.
  static void enableMessages();
  /// Disable all messages.
  static void disableMessages();
  /// Set the string which precedes all messages at writing-out time.
  static void setPrefix(const std::string& newPrefix);

private:
  /// Write the context (like '*** MyClass: ERROR: ') to the stream
  void writeContext(std::ostream&) const; 
  /// @brief Split the message string into one or more tokens. The boundaries
  /// are defined by the positions of the $1, $2 passed in by the user
  void tokeniseMessage();

private:
  /// Produce a string which corresponds to the severity level.
  static std::string severityString(const Severity& sev);
  
private:
  static const int MAX_TOKENS;
  static const std::string TOKEN_DELIMITER[];
  static const std::string DEFAULT_PREFIX;
  static const std::string DEFAULT_MESSAGE;
  /// See verbosity. 
  static const Severity DEFAULT_SEVERITY;
  static const Severity DEFAULT_VERBOSITY;
  static bool silent;
  static std::ostream* theStream;
  static std::string prefix;
  /// @brief All messages with a severity less than this will NOT be 
  /// printed (so DEBUG is not printed by default).
  static Severity verbosity;

private:
  typedef std::vector<std::string> SVec;
  typedef SVec::const_iterator SVecCIt;

private:
  Severity    m_severity;
  std::string m_message;
  std::string m_context;
  /// The message string after splitting into tokens.
  SVec messageTokens;

};


#include "TextMessage.templ"


} // end of namespace AidaNative


#endif // AIDANATIVE_TEXT_MESSAGE_H

