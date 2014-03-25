{
  'targets': [{
    'target_name': 'hoedown',
    'sources': [
      'src/hoedown/autolink.c',
      'src/hoedown/buffer.c',
      'src/hoedown/document.c',
      'src/hoedown/escape.c',
      'src/hoedown/html_blocks.c',
      'src/hoedown/html.c',
      'src/hoedown/html_smartypants.c',
      'src/hoedown/stack.c',
      'src/hoedown/version.c',

      'src/binding.cc'
    ],

    # Flags and defines
    'cflags': ['-Wall','-Wextra','-Wno-unused-paramter','-O3'],
  }]
}
