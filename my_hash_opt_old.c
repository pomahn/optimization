#include "hashtable.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//result of hash func -> eax


static unsigned hash(const char* str) {
    asm(".intel_syntax noprefix\n\t"
        "mov eax, 0\n\t" 
        "loop_cond:\n\t" //register for strlen of word
        "xor r8, r8\n\t" 
        "movzx r8, byte ptr [rdi]\n\t" 
	"inc ecx\n\t" // ecx -strlen counter  
        "test r8b, r8b\n\t" 
        "je loop_end\n\t" 
        "add eax, r8d\n\t" 
        "mov r8d, eax\n\t" 
        "shl r8d, 13\n\t" 
        "add eax, r8d\n\t" 
        "xor r15, r15 \n\t" 
        "mov r15d, eax\n\t" 
        "shr r15d, 19\n\t" 
        "or r8d, r15d\n\t" 
        "mov eax, r8d\n\t" 
        "inc rdi\n\t" 
        "jmp loop_cond\n\t" 
        "loop_end:\n\t" 
        "mov ebx, 3007\n\t" 
        "xor edx, edx\n\t" 
        "div ebx\n\t" 
        "mov eax, edx\n\t"
	".att_syntax\n\t"
	:
	:
	:"%eax", "%rbx", "%rdx", "%rdi", "%r15", "%r8", "%rcx"
    );
}


HtItem* ht_search(HashTable* ht, const char* s) {
    asm(".intel_syntax noprefix\n\t"
        "mov r9, rdi\n\t" // move ht to r9
        "mov r10, rsi\n\t" // move s to r10
        "mov rdi, r10\n\t" //s to rdi
	
        "mov eax, 0\n\t" 
        "loop_cond:\n\t" //register for strlen of word
        "xor r8, r8\n\t" 
        "movzx r8, byte ptr [rdi]\n\t" 
	"inc ecx\n\t" // ecx -strlen counter  
        "test r8b, r8b\n\t" 
        "je loop_end\n\t" 
        "add eax, r8d\n\t" 
        "mov r8d, eax\n\t" 
        "shl r8d, 13\n\t" 
        "add eax, r8d\n\t" 
        "xor r15, r15 \n\t" 
        "mov r15d, eax\n\t" 
        "shr r15d, 19\n\t" 
        "or r8d, r15d\n\t" 
        "mov eax, r8d\n\t" 
        "inc rdi\n\t" 
        "jmp loop_cond\n\t" 
        "loop_end:\n\t" 
        "mov ebx, 3007\n\t" 
        "xor edx, edx\n\t" 
        "div ebx\n\t" 
        "mov eax, edx\n\t"
	
       "mov r8, [eax*8+r9]\n\t"//there is first elem with an appropriate hash
	".loop_search:\n\t"
	"test r8, r8\n\t"
	"je .loop_search_end\n\t"
	"mov rbx, [r8+8]\n\t"//*(char*)
	"xor rcx, rcx\n\t"
	"mov rsi, r10\n\t"
	"mov rdi, rbx\n\t"
	"cld\n\t"
	"repe cmpsb \n\t"
	"je .loop_search_end\n\t"

	".not_equal:\n\t"
	"mov r8, [r8]\n\t"
	"jmp .loop_search\n\t"
	".loop_search_end:\n\t"

        ".att_syntax\n\t"
        :
        :
        : "%rax", "%r9", "%r10", "%rdi", "%rsi", "%r11", "%r12", "%r13", "%r14"
    );
}

int ht_insert(HashTable* ht, const char* s) {
    if (ht_search(ht, s) == NULL) {
        HtItem* item = (HtItem*)calloc(1, sizeof(*item));
        unsigned h = hash(s);
        item->next = ht->buckets[h];
        item->str  = s;
        ht->buckets[h] = item;
    }
    return 0;
}

