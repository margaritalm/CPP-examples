struct m68hc11_reloc_map
{
      
	 unsigned char code;
      unsigned char offset;
      unsigned short raddr;
      unsigned long old_offset;
	  char code2;
       char offset2;
       short raddr2;
       long old_offset2;
      
  bfd_reloc_code_real_type bfd_reloc_val;
  unsigned char elf_reloc_val;
};
