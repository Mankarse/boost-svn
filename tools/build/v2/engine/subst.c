#include <stddef.h>
#include "jam.h"
#include "regexp.h"
#include "hash.h"

#include "object.h"
#include "lists.h"
#include "compile.h"
#include "frames.h"
#include "builtins.h"

struct regex_entry
{
    OBJECT* pattern;
    regexp* regex;
};
typedef struct regex_entry regex_entry;

static struct hash* regex_hash;

regexp* regex_compile( OBJECT* pattern )
{
    int found;
    regex_entry * e ;

    if ( !regex_hash )
        regex_hash = hashinit(sizeof(regex_entry), "regex");

    e = (regex_entry *)hash_insert( regex_hash, pattern, &found );
    if ( !found )
    {
        e->pattern = object_copy( pattern );
        e->regex = regcomp( (char*)pattern );
    }

    return e->regex;
}

LIST * builtin_subst( FRAME * frame, int flags )
{
  LIST* result = L0;
  LIST* arg1 = lol_get( frame->args, 0 );

  if ( arg1 && list_next(arg1) && list_next(list_next(arg1)) )
  {

      const char* source = object_str( arg1->value );
      OBJECT * pattern = list_next(arg1)->value;
      regexp* repat = regex_compile( pattern );

      if ( regexec( repat, (char*)source) )
      {
          LIST* subst = list_next(arg1);

          while ((subst = list_next(subst)) != L0)
          {
# define BUFLEN 4096
              char buf[BUFLEN + 1];
              const char* in = object_str( subst->value );
              char* out = buf;

              for ( in = object_str( subst->value ); *in && out < buf + BUFLEN; ++in )
              {
                  if ( *in == '\\' || *in == '$' )
                  {
                      ++in;
                      if ( *in == 0 )
                      {
                          break;
                      }
                      else if ( *in >= '0' && *in <= '9' )
                      {
                          unsigned n = *in - '0';
                          const size_t srclen = repat->endp[n] - repat->startp[n];
                          const size_t remaining = buf + BUFLEN - out;
                          const size_t len = srclen < remaining ? srclen : remaining;
                          memcpy( out, repat->startp[n], len );
                          out += len;
                          continue;
                      }
                      /* fall through and copy the next character */
                  }
                  *out++ = *in;
              }
              *out = 0;

              result = list_new( result, object_new( buf ) );
#undef BUFLEN
          }
      }
  }

  return result;
}


static void free_regex( void * xregex, void * data )
{
    regex_entry * regex = (regex_entry *)xregex;
    object_free( regex->pattern );
    BJAM_FREE( regex->regex );
}


void regex_done()
{
    if ( regex_hash )
    {
        hashenumerate( regex_hash, free_regex, (void *)0 );
        hashdone( regex_hash );
    }
}
