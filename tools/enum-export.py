#!/usr/bin/python
# Tool that produces the C++ code to export a set of
# enum symbols to the JS side, with appropiate names.
#
# This version produces code to store these symbols
# in a hash, and to initialize the hash itself.
#
# First reads lines from stdin,
# then outputs the code to stdout.
from sys import stdin, stdout, stderr, argv

# ADJUST SETTINGS:
strip = "HOEDOWN"
if argv[1] not in ("", "-"):
  strip += '_'+argv[1]
hash = argv[2]
hash_sym = argv[3] if (len(argv) > 3) else None
hash_var = argv[4] if (len(argv) > 4) else None

import re
stderr.write('\n'.join(['Strip: {}', 'Hash: {}', 'Hash name: {}', 'Hash C var: {}']).format(strip, hash, hash_sym or '<auto>', hash_var or '<auto>')+'\n\n')

# CODE SETTINGS:
indent = 4
pre_pattern = 'Local<Object> {hash_var} = NanNew<Object>();'
out_pattern = '{hash_var}->Set(NanNew("{js_sub_sym}"), NanNew<Integer>({c_sym}));'
post_pattern = 'target->Set(NanNew("{hash_sym}"), {hash_var});'


# Prepare patterns
prepare_pattern = lambda pattern: (' '*indent)+pattern+'\n'

pre_pattern = prepare_pattern(pre_pattern)
out_pattern = prepare_pattern(out_pattern)
post_pattern = prepare_pattern(post_pattern)

def capital(case):
  ret=str()
  i=0; l=len(case)
  while i<l:
    if not i: ret += hash[i].upper()
    elif case[i] == '_':
      i+=1
      ret += hash[i]
    else: ret += hash[i].lower()
    i+=1
  return ret

if not hash_sym: hash_sym = capital(hash)
if not hash_var: hash_var = hash_sym[0].lower() + hash_sym[1:] + 'Hash'

strip += '_'; hash += '_'

excl = re.compile('^\s*$')
patt = re.compile('^\s*(\w+)(\s*=.+)?\s*(,?)\s*$')

# Begin processing
output=pre_pattern.format(hash_var=hash_var)
try:
  while True:
    line = raw_input()
    if re.match(excl, line): continue
    
    m = re.match(patt, line)
    if not m: raise Exception('bad line!')

    c_sym = m.group(1)
    if not c_sym.startswith(strip): raise Exception('bad strip!')

    js_sym = c_sym[len(strip):]
    if not js_sym.startswith(hash): raise Exception('bad hash!')

    js_sub_sym = js_sym[len(hash):]
    output += out_pattern.format(hash_var=hash_var, c_sym=c_sym, js_sub_sym=js_sub_sym)

except EOFError, e: pass
#except Exception, e:
#  print 'Error: ' + e

output += post_pattern.format(hash_var=hash_var, hash_sym=hash_sym)
stdout.write('\n'+output+'\n')
