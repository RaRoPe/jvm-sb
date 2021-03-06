#ifndef LEITOR_EXIBIDOR_H
#define LEITOR_EXIBIDOR_H

/*Includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h> 

#include "decodificador.h"

/*Defines*/
#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1

#define Public    0x0001
#define Private   0x0002
#define Protected 0x0004
#define Static    0x0008
#define Final     0x0010
#define Super     0x0020
#define Volatile  0x0040
#define Transient 0x0080
#define Native    0x0100
#define Interface 0x0200
#define Abstract  0x0400
#define Strict    0x0800

#define NUM_INSTRUCAO 256

// opcodes para instrucoes especiais
#define TABLESWITCH 170
#define LOOKUPSWITCH 171
#define WIDE 196

// opcodes que o wide precisa
#define ILOAD 21
#define FLOAD 23
#define ALOAD 25
#define LLOAD 22
#define DLOAD 24
#define ISTORE 54
#define FSTORE 56
#define ASTORE 58
#define LSTORE 55
#define DSTORE 57
#define RET 169

#define IINC 132

/* Structs */
typedef struct attribute_info{
	uint16_t attribute_name_index;
	uint32_t attribute_length;
	uint8_t* info;
}attribute_info;

/* structs que representam os atributos do programa*/
typedef struct ConstantValue_attribute {
	uint16_t attribute_name_index;
	uint32_t attribute_length;
	uint16_t constantvalue_index;
}CV_info;

// struct da tabela de excecoes, util para a struct dos atributos do codigo 
typedef struct exception_table
{
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t catch_type; 
}exception_table;

// struct para atributos do tipo codigo
typedef struct code_attribute 
{
    uint16_t attribute_name_index;
    uint32_t attribute_length; 
    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_length; 
    exception_table* exception_table; 
    uint16_t attributes_count;
    attribute_info* attributes; 
}code_attribute;

// struct para atributos do tipo exceptions
typedef struct exceptions_attribute
{
    uint16_t attribute_name_index;
    uint32_t attribute_length; 
    uint16_t number_of_exceptions;
    uint16_t* exception_index_table; 
}exceptions_attribute; 

typedef struct field_info{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	CV_info* attributes;
}field_info;

typedef struct method_info{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	code_attribute* cd_atrb;
    exceptions_attribute* exc_atrb; // nao eh garantido que vai ter excecao 
}method_info;

/*Struct de informações da constant pool*/
typedef struct cp_info{
	uint8_t tag;
	union{
		struct {
			uint16_t name_index;
		}Class;
		struct{
			uint16_t class_index;
			uint16_t name_and_type_index;
		}Fieldref;
		struct{
			uint16_t name_index;
			uint16_t descriptor_index;
		}NameAndType;
		struct{
			uint16_t length;
			uint8_t* bytes;
		}Utf8;
		struct{
			uint16_t class_index;
			uint16_t name_and_type_index;
		}Methodref;
		struct{
			uint16_t class_index;
			uint16_t name_and_type_index;
		}InterfaceMethodref;
		struct{
			uint16_t string_index;
		}String;
		struct{
			uint32_t bytes;
		}Integer;
		struct{
			uint32_t bytes;
		}Float;
		struct{
			uint32_t high_bytes;
			uint32_t low_bytes;
		}Long;
		struct{
			uint32_t high_bytes;
			uint32_t low_bytes;
		}Double;
	}info;
}cp_info;

/*Struct que representa o .class*/
typedef struct ClassFile{
	uint32_t magic;
	uint16_t minor_version;
	uint16_t major_version;
	uint16_t constant_pool_count;
	cp_info* constant_pool;
	uint16_t access_flags;
	uint16_t this_class;
	uint16_t super_class;
	uint16_t interfaces_count;
	uint16_t* interfaces;
	uint16_t fields_count;
	field_info* fields;
	uint16_t methods_count;
	method_info* methods;
	uint16_t attributes_count;
	attribute_info* attributes;
}classFile;

/* Funcoes do programa */

// funcao principal
int main(int argc, char* argv[]);

// Função que le informações gerais do arquivo - Antes da constant pool
void generalInfo(classFile* cf,FILE* file);

// funcao para ler constant pool
void constantPool(classFile* cf,FILE* file);

// Função para ler estrutura e informações dos métodos.
void methodInfo(classFile* cf, FILE* file, uint16_t methods_count);

// Função para ler estrutura e informações de atributos.
void attributeInfo(classFile* cf, FILE* file, uint16_t attributes_count);

// funcao para ler informacoes gerais que vem depois da constante pool 
void secondGeneralInfo(classFile* cf,FILE* file);

void imprimePrompt(classFile* cf);
void le_exc(exceptions_attribute** exc_atrb, uint16_t name_ind, uint32_t att_len, FILE* file);
void le_code(code_attribute** cd_atrb, uint16_t name_ind, uint32_t att_len, FILE* file);
void salva_instrucoes(code_attribute** cd_atrb, FILE* file);

classFile* leitorClasse(char * nomeClass);

// funcoes auxiliares
static inline uint8_t le1Byte(FILE* fp);
static inline uint16_t le2Bytes(FILE* fp);
static inline uint32_t le4Bytes(FILE* fp);

#endif 
