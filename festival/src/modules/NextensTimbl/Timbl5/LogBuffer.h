#ifndef LOGBUFFER_H
#define LOGBUFFER_H
/* Copyright (c) 1998 - 2006
 * ILK  -  Tilburg University
 * CNTS -  University of Antwerp
 *
 * All rights Reserved.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * For questions and suggestions, see:
 *	http://ilk.uvt.nl/software.html
 * or send mail to:
 *	Timbl@uvt.nl
 */

enum LogLevel{ LogSilent, LogNormal, LogDebug, LogHeavy, LogExtreme };
enum LogFlag { NoStamp=0, StampTime=1, StampMessage=2, StampBoth=3 };

#if __GNUC__ < 3
// braindead gcc
#include <iomanip>
#include <iostream>

class LogBuffer : public std::streambuf {
 public:
  LogBuffer( std::ostream& , const char * = NULL, 
	     const LogFlag = StampBoth );
  ~LogBuffer();
  //
  // setters/getters
  LogLevel Level() const;
  void Level( const LogLevel l );
  LogLevel Treshold() const;
  void Treshold( const LogLevel l );
  const char *Message() const;
  void Message( const char* );
  LogFlag StampFlag() const;
  void StampFlag( const LogFlag );
  std::ostream& AssocStream() const;
  void AssocStream( std::ostream & );
 protected:
  int sync();
  int overflow( int );
 private:
  std::ostream *ass_stream;
  LogFlag stamp_flag;
  bool in_sync;
  LogLevel level;
  LogLevel treshold_level;
  char *ass_mess;
  void buffer_out();
  // prohibit copying and assignment
  LogBuffer( const LogBuffer& );
  LogBuffer& operator=( const LogBuffer& );
};

#else // __GNUC__

#include <ctime>
#include <cstring>
#include <cstdio>
#include <typeinfo>
#include <iomanip>
#include <iostream>
#if defined ( PTHREADS ) && !defined ( _REENTRANT )
#define _REENTRANT
#endif
#include <sys/time.h>
#include <unistd.h>

template <class charT, class traits = std::char_traits<charT> >
class basic_log_buffer : public std::basic_streambuf<charT, traits> {
 public:
  basic_log_buffer( std::basic_ostream<charT,traits>&, const char * = NULL, 
		    const LogFlag = StampBoth );
  ~basic_log_buffer();
  //
  // setters/getters
  LogLevel Level() const;
  void Level( const LogLevel l );
  LogLevel Treshold() const;
  void Treshold( const LogLevel l );
  const char *Message() const;
  void Message( const char* );
  std::basic_ostream<charT,traits>& AssocStream() const;
  void AssocStream( std::basic_ostream<charT,traits>& );
  LogFlag StampFlag() const;
  void StampFlag( const LogFlag );
 protected:
  int sync();
  int overflow( int );
 private:
  std::basic_ostream<charT,traits> *ass_stream;
  LogFlag stamp_flag;
  bool in_sync;
  LogLevel level;
  LogLevel treshold_level;
  char *ass_mess;
  void buffer_out();
  // prohibit copying and assignment
  basic_log_buffer( const basic_log_buffer& );
  basic_log_buffer& operator=( const basic_log_buffer& );
};

typedef basic_log_buffer<char, std::char_traits<char> > LogBuffer;
typedef basic_log_buffer<wchar_t, std::char_traits<wchar_t> > wLogBuffer;

template <class charT, class traits >
basic_log_buffer<charT,traits>::basic_log_buffer( std::basic_ostream<charT,traits>& a,
						  const char *mess, 
						  const LogFlag stamp ) {
  ass_stream = &a;
  if ( mess ){
    ass_mess = new char[strlen(mess)+1];
    strcpy( ass_mess, mess );
  }
  else
    ass_mess = NULL;
  stamp_flag = stamp;
  in_sync = true;
  level = LogNormal;
  treshold_level = LogSilent;
}

template <class charT, class traits >
basic_log_buffer<charT,traits>::~basic_log_buffer(){
  sync();
  delete [] ass_mess;
}

inline long millitm() {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  return tp.tv_usec/1000;
}

inline char *time_stamp( char *time_line, int size ){
  time_t lTime;
  struct tm *curtime;
  time(&lTime);
#if defined __sun__ || defined __osf__
  struct tm tmp;
  curtime = localtime_r(&lTime,&tmp);
#else
  curtime = localtime(&lTime);
#endif
  strftime( time_line, size-5, "%Y%m%d:%H%M%S", curtime );
  sprintf( time_line+strlen(time_line), ":%03ld:", millitm() );
  return time_line;
}

//
// for a derived output stream, we must provide implementations for
// both overflow and sync.
// both use a helper function buffer_out to do the real work.
//

template <class charT, class traits >
int basic_log_buffer<charT,traits>::overflow( int c ) {
  buffer_out();
  if ( level > treshold_level && c != '\r' )
    ass_stream->put( c );
  return c;
}

template <class charT, class traits >
int basic_log_buffer<charT,traits>::sync() {
  ass_stream->flush();
  in_sync = true;
  return 0;
}

template <class charT, class traits >
void basic_log_buffer<charT,traits>::buffer_out(){
  char time_line[50];
  if ( level > treshold_level ){
    // only output when we are on a high enough level
    if ( in_sync ) {
      // stamps and messages are only displayed when in sync
      // that is: when we have had a newline and NOT when we just
      // overflowed due to a long line
      if ( stamp_flag & StampTime ){
	*ass_stream << time_stamp( time_line, 50 );
      }
      if ( ass_mess && ( stamp_flag & StampMessage ) )
	*ass_stream << ass_mess << ":";
      in_sync = false;
    }
  }
}

//
// Getters and Setters for the private parts..
//

template <class charT, class traits >
const char *basic_log_buffer<charT,traits>::Message() const {
  return ass_mess;
}

template <class charT, class traits >
void basic_log_buffer<charT,traits>::Message( const char *s ){
  delete [] ass_mess;
  if ( s ){
    ass_mess = new char[strlen(s)+1];
    strcpy( ass_mess, s );
  }
  else
    ass_mess = NULL;
}

template <class charT, class traits >
void basic_log_buffer<charT,traits>::Treshold( LogLevel l ){ 
  if ( treshold_level != l ){
    treshold_level = l;
  }
}

template <class charT, class traits >
LogLevel basic_log_buffer<charT,traits>::Treshold() const { 
  return treshold_level;
}

template <class charT, class traits >
void basic_log_buffer<charT,traits>::Level( LogLevel l ){ 
  if ( level != l ){
    level = l;
  }
}

template <class charT, class traits >
LogLevel basic_log_buffer<charT,traits>::Level() const { 
  return level;
}

template <class charT, class traits >
std::basic_ostream<charT,traits>& basic_log_buffer<charT,traits>::AssocStream() const {
  return *ass_stream;
}

template <class charT, class traits >
void basic_log_buffer<charT,traits>::AssocStream( std::basic_ostream<charT,traits>& os ){
  ass_stream = &os;
}

template <class charT, class traits >
void basic_log_buffer<charT,traits>::StampFlag( const LogFlag b ){ 
  if ( stamp_flag != b ){
    stamp_flag = b;
  }
}

template <class charT, class traits >
LogFlag basic_log_buffer<charT,traits>::StampFlag() const { 
  return stamp_flag;
}

#endif // __GNUC__

#endif // LOGBUFFER_H
