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
	
        "mov rax, [r9 + 8 * rax]\n\t"
        "searchloop_cond:\n\t"
        "test rax, rax\n\t"
        "je searchloop_end\n\t" // пустая ячейка
        "mov rdi, r10\n\t" //в рди строка
        "mov rsi, [rax+8]\n\t" // рси строка из элемента
        "mov r11, rax\n\t"  // указатель на элемент
        // strcmp memcmp with strlen ; repne cmpsb	rdi rsi - адреса для сравнения
	// repe использует ecx как счетчик zf как флаг. Не забыть CLD для правильного направления
        "xor r12, r12\n\t"
        "strcmp_cond:\n\t"
        "mov rax, 1\n\t"
        "mov r13, [rdi + r12]\n\t"
        "mov r14, [rsi + r12]\n\t"
        "cmp r13, r14\n\t"
        "jne strcmp_end\n\t"
        "inc r12\n\t"
        "cmp r13, 0\n\t"
        "jne  strcmp_cond\n\t"
        "mov rax, 0\n\t"
        "strcmp_end:\n\t"
        "mov esi, eax\n\t"   
        "mov rax, r11\n\t" //указатель на эремент в рах
        "test esi, esi\n\t"
        "je searchloop_end\n\t" // если не нашел до последнего элемента
        "mov rax, [rax]\n\t"  //переход к следуещему элементу
        "jmp searchloop_cond\n\t"
        "searchloop_end:\n\t"
        ".att_syntax\n\t"
        :
        :
        : "%rax", "%r9", "%r10", "%rdi", "%rsi", "%r11", "%r12", "%r13", "%r14"
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
	
        "mov rax, [r9 + 8 * rax]\n\t" // обращение к элементу таблицы
        "searchloop_cond:\n\t" //
        "test rax, rax\n\t"
        "je searchloop_end\n\t" // пустая ячейка
        "mov rdi, r10\n\t" //в рди строка
        "mov rsi, [rax+8]\n\t" // рси строка из элемента
        "mov r11, rax\n\t"  // указатель на элемент
        // strcmp memcmp with strlen ; repne cmpsb	rdi rsi - адреса для сравнения
	// repne использует ecx как счетчик zf как флаг. Не забыть CLD для правильного направления
        "xor r12, r12\n\t"
        "strcmp_cond:\n\t"
        "mov rax, 1\n\t"
        "mov r13, [rdi + r12]\n\t"
        "mov r14, [rsi + r12]\n\t"
        "cmp r13, r14\n\t"
        "jne strcmp_end\n\t"
        "inc r12\n\t"
        "cmp r13, 0\n\t"
        "jne  strcmp_cond\n\t"
        "mov rax, 0\n\t"
        "strcmp_end:\n\t"
        "mov esi, eax\n\t"   
        "mov rax, r11\n\t" //указатель на эремент в рах
        "test esi, esi\n\t"
        "je searchloop_end\n\t" // если не нашел до последнего элемента
        "mov rax, [rax]\n\t"  //переход к следуещему элементу
        "jmp searchloop_cond\n\t"
        "searchloop_end:\n\t"
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

