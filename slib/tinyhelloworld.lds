ENTRY(nomain)

SECTIONS
{
   . = 0x0804800 + SIZEOF_HEADERS;
   tinytext : {  *(.eh_frame) *(.text)  * (.data) *(.rodata)  }
   /DISCARD/ : { *(.comment)}
}
